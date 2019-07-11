#include "adc.h"
#include "../dac.h"
#include "pos_esti.h"

#define sense_buffer 50
#define BEMF_buffer 3
#define SAM_F 2454

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
ADC_InitTypeDef ADC_InitStru;
DMA_InitTypeDef DMA_InitStru;

volatile s16 ADC_buffer[3][sense_buffer] = {0};
s16 peak_to_peak[3] = {0};
volatile u16 reading[1024] = {0};
volatile bool flag = 0; // 0 not occupy ; 1 occupy
u8 curr_input=0;
u16 ADC_MEAN[3]={0};
s16 HIF_BEMF[3]={0};


s16 fil_array[3][10]={0};
u32 Gau[10]={10,99,96,91,85,77,69,61,52,44};
u32 sum_gau = 774;
u32 gau_count[3]={0};


s16 bemf_array[3][8]={0};
u32 Gaub[8]={100,92,71,46,25,11,4,1};
u32 sum_gaub = 350;
u32 gaub_count[3]={0};

u8 FUCKYOU = 0;


u16 Gaussian_filter(s16 input,u8 channel){
	u32 result = 0;
	fil_array[channel][gau_count[channel]%10] = input;
	for(u8 i=0;i<10;i++){
		result += Gau[i]*fil_array[channel][(gau_count[channel]-i)%10];
	}
	gau_count[channel]++;
	
	if(gau_count[channel]<10){
		return input;
	}
	
	return (u16)(result/sum_gau);

}

u16 Gaubian_filter(s16 input,u8 channel){
	u32 result = 0;
	bemf_array[channel][gaub_count[channel]%8] = input;
	for(u8 i=0;i<8;i++){
		result += Gaub[i]*bemf_array[channel][(gaub_count[channel]-i)%8];
	}
	gaub_count[channel]++;
	
	if(gaub_count[channel]<8){
		return input;
	}
	
	return (u16)(result/sum_gaub);

}





void DMA2_Stream0_IRQHandler(void) { //ADC DMA interrupt handler
	 DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
	 flag=0;
}	



void adc_gpio_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	

}

void TIM8_init(void){
  
       	
	TIM_DeInit(TIM8);
     
// TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);

  TIM_Cmd(TIM8, ENABLE);	

}

void adc_bemf_init(u8 input){

	
	//DMA_Cmd(DMA2_Stream1, DISABLE);
	DMA2_Stream1->CR &= ~(uint32_t)DMA_SxCR_EN;
	ADC2->CR2 &= (uint32_t)(~(ADC_CR2_DDS|ADC_CR2_DMA|ADC_CR2_ADON));
	
	ADC_DeInit();
	ADC_RegularChannelConfig(ADC1, 5+input , 1, ADC_SampleTime_3Cycles); // here to change the reading pin
	ADC_Init(ADC1, &ADC_InitStru);
	// specific for adc initi
	DMA_InitStru.DMA_Memory0BaseAddr = (uint32_t) &(ADC_buffer[input]);
	DMA_InitStru.DMA_BufferSize = BEMF_buffer;
	DMA_Init(DMA2_Stream0, &DMA_InitStru); // DMA2 ADC1 S0C0  ADC2 S2C1 ADC3 S0C2
	
	
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

	DMA2_Stream0->CR |= (uint32_t)DMA_SxCR_EN;
	ADC1->CR2 |= (uint32_t)(ADC_CR2_DDS|ADC_CR2_DMA|ADC_CR2_ADON|ADC_CR2_SWSTART);
	ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;

}


void adc_dma_init(u8 input,u8 buff){


	TIM8_init();
	
	
	DMA2_Stream1->CR &= ~(uint32_t)DMA_SxCR_EN;
	ADC3->CR2 &= (uint32_t)(~(ADC_CR2_DDS|ADC_CR2_DMA|ADC_CR2_ADON));
	ADC3->CR2 &= (uint32_t)(~ADC_CR2_SWSTART);
	//ADC2->CR2 &= (uint32_t)(~ADC_CR2_DMA);
	//ADC2->CR2 &= (uint32_t)(~ADC_CR2_ADON);
	
	
	ADC_DeInit();


	ADC_RegularChannelConfig(ADC1, input , 1, ADC_SampleTime_3Cycles); // here to change the reading pin
	
	
	
	ADC_Init(ADC1, &ADC_InitStru);
	// specific for adc initi
	DMA_InitStru.DMA_Memory0BaseAddr = (uint32_t) &(ADC_buffer[input]);
	if(buff == 1){
	DMA_InitStru.DMA_BufferSize = sense_buffer;
	}else{
	DMA_InitStru.DMA_BufferSize = 5;
	}
	DMA_Init(DMA2_Stream0, &DMA_InitStru); // DMA2 ADC1 S0C0  ADC2 S2C1 ADC3 S0C2
	
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	
	DMA2_Stream0->CR |= (uint32_t)DMA_SxCR_EN;
	ADC1->CR2 |= (uint32_t)(ADC_CR2_DDS|ADC_CR2_DMA|ADC_CR2_ADON|ADC_CR2_SWSTART);
	ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;

}





void adc_init(void){
	ADC_CommonInitTypeDef ADC_CommonInitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
	
	adc_gpio_init();
		
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	
	TIM_TimeBaseStructure.TIM_Period =1;        //100 => 13k 20=>62.5k 30=>42.37k  10=>120k      9=>130
  TIM_TimeBaseStructure.TIM_Prescaler = 17;                                                               
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;                                                   
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 


	ADC_InitStru.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStru.ADC_ScanConvMode = DISABLE;
	ADC_InitStru.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStru.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_CC1;
	ADC_InitStru.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStru.ADC_DataAlign = ADC_DataAlign_Right;
	
	// common init
	DMA_InitStru.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStru.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStru.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStru.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStru.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStru.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStru.DMA_Priority = DMA_Priority_High;
	DMA_InitStru.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStru.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStru.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStru.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	ADC_InitStru.ADC_NbrOfConversion = 1;
	DMA_InitStru.DMA_Channel =  DMA_Channel_0;
	DMA_InitStru.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1)->DR;
	
	//ADC init
	adc_dma_init(1,1);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

}



bool adc_done(void){
	
	if(flag == 1){return false;}
	return true;
	//return !flag;
}


void reset_dma_adc(u8 input,u8 buff){
		//if(flag){return;}
		flag = 1;
		curr_input=input;
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
		adc_dma_init(input,buff);
		return ;
}

void reset_dma_adc_bemf(u8 input){
		//if(flag){return;}
		flag = 1;
		curr_input=input;
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
		adc_bemf_init(input);
		return ;
}




s32 median(u8 n, s32* x) {
    s32 temp;
    u8 i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) >>1);
    } else {
        // else return the element in the middle
        return x[n/2];
    }
}

void pk2pk2(u8 input){
	
	if(FUCKYOU){
		uart_tx(COM3,"wave=[");
			for (u32 i=0;i<sense_buffer;i++){
		uart_tx(COM3,"%d,",ADC_buffer[input][i]);
		_delay_ms(1);
	}
	uart_tx(COM3,"];plot(wave(2:%d))\n",sense_buffer);
}
	
	
	
	
	u16 zero_count = 0;
	u16 freq = get_interval()-1;
	
	u8 peak_num[2]={0,0};
	//s32 peak[2][12]={0,0};
	s32 peak2[2] = {0};
	ADC_buffer[input][2] = ADC_buffer[input][2]-ADC_buffer[input][6];
	ADC_buffer[input][3] = ADC_buffer[input][3]-ADC_buffer[input][7];
	for(u16 i=3;i<sense_buffer-6;i++){ 
	 if(ADC_buffer[input][i+1]==0){
			zero_count++;
		 if(zero_count>10){
				return ;
			}
		}
		ADC_buffer[input][i+1] = ADC_buffer[input][i+1]-ADC_buffer[input][i+5];
		if(ADC_buffer[input][i-1]>=ADC_buffer[input][i]&&ADC_buffer[input][i+1]>=ADC_buffer[input][i]){ //min
			peak_num[0]++;
			peak2[0]+=ADC_buffer[input][i];
			i+=freq;
			ADC_buffer[input][i] = ADC_buffer[input][i]-ADC_buffer[input][i+4];
			ADC_buffer[input][i+1] = ADC_buffer[input][i+1]-ADC_buffer[input][i+5];
		}
		else if(ADC_buffer[input][i-1]<=ADC_buffer[input][i]&&ADC_buffer[input][i+1]<=ADC_buffer[input][i]){ //max
			peak_num[1]++;
			peak2[1]+=ADC_buffer[input][i];
			i+=freq;
			ADC_buffer[input][i] = ADC_buffer[input][i]-ADC_buffer[input][i+4];
			ADC_buffer[input][i+1] = ADC_buffer[input][i+1]-ADC_buffer[input][i+5];		}
	}
	
	peak2[0]/=peak_num[0];
	peak2[1]/=peak_num[1];
	

	//peak_to_peak[input] = ((peak2[1]-peak2[0])*SAM_F*100)/(get_freq()*8*314);
	peak_to_peak[input] = ((peak2[1]-peak2[0]))>>2;
	peak_to_peak[input]=Gaussian_filter(peak_to_peak[input],input);
	
	if(FUCKYOU){
		uart_tx(COM3,"\n %d \n",peak_to_peak[input]);
	}
	
	//HIF_BEMF[input] = sense_buffer*(peak2[1]+peak2[0])/8;
	
/*	if (diu==1&&input==0){
		uart_tx(COM3,"%d,",peak_to_peak[input]);
	}*/

}


void bemf(u8 input){
	peak_to_peak[input] =((ADC_buffer[input][1]+ADC_buffer[input][2])>>1);//-ADC_MEAN[input];
	peak_to_peak[input]=Gaubian_filter(peak_to_peak[input],input);
}


void pk2pk(u8 input){

/*	uart_tx(COM3,"adc=[");
	for (u8 i=0;i<sense_buffer;i++){
		uart_tx(COM3,"%d,",ADC_buffer[input][i]);
		_delay_ms(1);
	}
	uart_tx(COM3,"];\n");*/
	
	u16 zero_count = 0;
	u16 freq = get_interval()-1;
	
	u8 peak_num[2]={0,0};
	//s32 peak[2][12]={0,0};
	u32 peak2[2] = {0};
	ADC_buffer[input][2] = ADC_buffer[input][2]+ADC_buffer[input][3]+ADC_buffer[input][4]+ADC_buffer[input][5];
	ADC_buffer[input][3] = ADC_buffer[input][3]+ADC_buffer[input][4]+ADC_buffer[input][5]+ADC_buffer[input][6];
	for(u16 i=3;i<sense_buffer-4;i++){ 
	 if(ADC_buffer[input][i+1]==0){
			zero_count++;
		 if(zero_count>10){
				//DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
				//adc_dma_init(curr_input);
				//while(1){}
				return ;
			}
		}
		ADC_buffer[input][i+1] = ADC_buffer[input][i+1]+ADC_buffer[input][i+2]+ADC_buffer[input][i+3]+ADC_buffer[input][i+4];
		if(ADC_buffer[input][i-1]>=ADC_buffer[input][i]&&ADC_buffer[input][i+1]>=ADC_buffer[input][i]){ //min
			peak_num[0]++;
			peak2[0]+=ADC_buffer[input][i];
			i+=freq;
			ADC_buffer[input][i] = ADC_buffer[input][i]+ADC_buffer[input][i+1]+ADC_buffer[input][i+2]+ADC_buffer[input][i+3];
			ADC_buffer[input][i+1] = ADC_buffer[input][i+1]+ADC_buffer[input][i+2]+ADC_buffer[input][i+3]+ADC_buffer[input][i+4];
		}
		else if(ADC_buffer[input][i-1]<=ADC_buffer[input][i]&&ADC_buffer[input][i+1]<=ADC_buffer[input][i]){ //max
			peak_num[1]++;
			peak2[1]+=ADC_buffer[input][i];
			i+=freq;
		  ADC_buffer[input][i] = ADC_buffer[input][i]+ADC_buffer[input][i+1]+ADC_buffer[input][i+2]+ADC_buffer[input][i+3];
		 ADC_buffer[input][i+1] = ADC_buffer[input][i+1]+ADC_buffer[input][i+2]+ADC_buffer[input][i+3]+ADC_buffer[input][i+4];
		}
	}
	
	peak2[0]/=peak_num[0];
	peak2[1]/=peak_num[1];
	

	peak_to_peak[input] = (peak2[1]-peak2[0])>>2;
	
/*	if(peak_to_peak[input]>1000){
		
		for(u16 i=0;i<sense_buffer;i++){
			uart_tx(COM3,"%d,",ADC_buffer[input][i]);
			_delay_ms(10);
		}
		uart_tx(COM3,"\n");
		uart_tx(COM3,"%d %d\n",peak_num[0],peak_num[1]);
		uart_tx(COM3,"%d %d\n",peak2[0],peak2[1]);
		while(1){}
	
	}*/

//	reading[get_abs()]=(peak_to_peak);
}

s16 get_pk2pk(u8 input){
	pk2pk2(input);
	//uart_tx(COM3,"%d;",peak_to_peak[input]);
	return (peak_to_peak[input]);
}

s16 get_bemf(u8 input){

		bemf(input);
	//uart_tx(COM3,"%d;",peak_to_peak[input]);
		return (peak_to_peak[input]);

}


void uart_reading(void){
	
	if(get_abs()==120){
	uart_tx(COM3,"y=[");
	for (u16 i=0;i<sense_buffer;i++){
		uart_tx(COM3,"%d,",ADC_buffer[0][i]);
		_delay_ms(1);
	}	
	uart_tx(COM3,";");
	for (u16 i=0;i<sense_buffer;i++){
		uart_tx(COM3,"%d,",ADC_buffer[1][i]);
		_delay_ms(1);
	}	
	uart_tx(COM3,";");
	for (u16 i=0;i<sense_buffer;i++){
		uart_tx(COM3,"%d,",ADC_buffer[2][i]);
		_delay_ms(1);
		
	}
	uart_tx(COM3,";");
	uart_tx(COM3,"];plot(1:%d,y(1:3,2:%d));\n",sense_buffer-1,sense_buffer);
  uart_tx(COM3,"%d %d %d;\n\n",get_pk2pk(0),get_pk2pk(1),get_pk2pk(2));
		
}
		

}


void ADC_midpt_cal(u8 input){
	//reset_dma_adc(10+input);
	while(!adc_done()){}
		
	u32 mean = 0;
	for (u8 i=0;i<sense_buffer;i++){
		mean+=ADC_buffer[input][i];
	}
	mean/=sense_buffer;
	ADC_MEAN[input] = mean;

	//uart_tx(COM3,"%d \n",mean);
}


void reset_filter_count(void){
	gau_count[0]=0;
	gau_count[1]=0;
	gau_count[2]=0;

}


void toggle_FUCK(void){
	if(FUCKYOU==1){
		FUCKYOU = 0;
		return;
	}
	
	FUCKYOU = 1;

}




