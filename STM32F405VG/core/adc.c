#include "adc.h"
#include "../dac.h"

#define sense_buffer 50
#define BEMF_buffer 3
#define SAM_F 2454
#define ADC_MEAN 2060


volatile s16 ADC_buffer[3][sense_buffer] = {0};
s16 peak_to_peak[3] = {0};
volatile u16 reading[1024] = {0};
volatile bool flag = 0; // 0 not occupy ; 1 occupy
volatile u32 count = 0;
u8 curr_input=0;


void DMA2_Stream0_IRQHandler(void) { //ADC DMA interrupt handler
	 DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
	 flag=0;
}	



void adc_gpio_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void TIM3_init(void){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_DeInit(TIM3);
     
// TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period =1;        //100 => 13k 20=>62.5k 30=>42.37k  10=>120k      9=>130
  TIM_TimeBaseStructure.TIM_Prescaler = 17;                                                               
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;                                                   
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;         
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

  TIM_Cmd(TIM3, ENABLE);	

}

void adc_bemf_init(u8 input){
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	ADC_DeInit();
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// common init
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	ADC_RegularChannelConfig(ADC1, input , 1, ADC_SampleTime_3Cycles); // here to change the reading pin
	
	
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	// specific for adc initi
	DMA_InitStructure.DMA_Channel =  DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1)->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &(ADC_buffer[input]);
	DMA_InitStructure.DMA_BufferSize = BEMF_buffer;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure); // DMA2 ADC1 S0C0  ADC2 S2C1 ADC3 S0C2
	
	
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);


}


void adc_dma_init(u8 input){
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	TIM3_init();
	
	ADC_DeInit();
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// common init
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	ADC_RegularChannelConfig(ADC1, input , 1, ADC_SampleTime_3Cycles); // here to change the reading pin
	
	
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	// specific for adc initi
	DMA_InitStructure.DMA_Channel =  DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1)->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &(ADC_buffer[input]);
	DMA_InitStructure.DMA_BufferSize = sense_buffer;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure); // DMA2 ADC1 S0C0  ADC2 S2C1 ADC3 S0C2
	
	

	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);

}





void adc_init(void){
	ADC_CommonInitTypeDef ADC_CommonInitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
	
	adc_gpio_init();
		
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//ADC init
	adc_dma_init(1);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

}



bool adc_done(void){
	return !flag;
}


void reset_dma_adc(u8 input){
		if(flag){return;}
		flag = 1;
		curr_input=input;
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);
		adc_dma_init(input);
		return ;
}

void reset_dma_adc_bemf(u8 input){
		if(flag){return;}
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
/*	u8 diu = 0;
	
	if (get_abs()==120&&input==0){
		uart_tx(COM1,"wave=[");
			for (u8 i=0;i<sense_buffer;i++){
		uart_tx(COM1,"%d,",ADC_buffer[input][i]);
		_delay_ms(1);
	}
			uart_tx(COM1,"];\n");
		diu=1;
	}
	*/
	
	
	
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
	

	peak_to_peak[input] = ((peak2[1]-peak2[0])*SAM_F*100)/(get_freq()*8*314);
	
/*	if (diu==1&&input==0){
		uart_tx(COM1,"%d,",peak_to_peak[input]);
	}*/

}


void bemf(u8 input){
	peak_to_peak[input] =((ADC_buffer[input][1]+ADC_buffer[input][2])>>1)-ADC_MEAN;
}


void pk2pk(u8 input){

	
	/*for (u8 i=0;i<sense_buffer;i++){
		uart_tx(COM1,"%d,",ADC_buffer[input][i]);
		_delay_ms(1);
	}*/
	
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
			uart_tx(COM1,"%d,",ADC_buffer[input][i]);
			_delay_ms(10);
		}
		uart_tx(COM1,"\n");
		uart_tx(COM1,"%d %d\n",peak_num[0],peak_num[1]);
		uart_tx(COM1,"%d %d\n",peak2[0],peak2[1]);
		while(1){}
	
	}*/

//	reading[get_abs()]=(peak_to_peak);
}

s16 get_pk2pk(u8 input){
	pk2pk2(input);
	//uart_tx(COM1,"%d;",peak_to_peak[input]);
	return (peak_to_peak[input]);
}

s16 get_bemf(u8 input){
	bemf(input);
	//uart_tx(COM1,"%d;",peak_to_peak[input]);
	return (peak_to_peak[input]);
}


void uart_reading(void){
	
	if(get_abs()==120){
	uart_tx(COM1,"y=[");
	for (u16 i=0;i<sense_buffer;i++){
		uart_tx(COM1,"%d,",ADC_buffer[0][i]);
		_delay_ms(1);
	}	
	uart_tx(COM1,";");
	for (u16 i=0;i<sense_buffer;i++){
		uart_tx(COM1,"%d,",ADC_buffer[1][i]);
		_delay_ms(1);
	}	
	uart_tx(COM1,";");
	for (u16 i=0;i<sense_buffer;i++){
		uart_tx(COM1,"%d,",ADC_buffer[2][i]);
		_delay_ms(1);
		
	}
	uart_tx(COM1,";");
	uart_tx(COM1,"];plot(1:%d,y(1:3,2:%d));\n",sense_buffer-1,sense_buffer);
  uart_tx(COM1,"%d %d %d;\n\n",get_pk2pk(0),get_pk2pk(1),get_pk2pk(2));
		
}
		

}


u32 get_count(void){
	return count;
}

