#include "main.h"


  #define ADC_CHANNEL              ADC_Channel_0
  #define ADC1_2_CHANNEL_GPIO_CLK  RCC_AHB1Periph_GPIOA
  #define GPIO_PIN                 GPIO_Pin_0
  #define GPIO_PORT                GPIOA
  #define DMA_CHANNELx             DMA_Channel_0
  #define DMA_STREAMx              DMA2_Stream0
  #define ADC_CDR_ADDRESS          ((uint32_t)0x40012308)



#define DELAY 10
#define TEST_ALGO pos_update_induc();
#define DURATION 1000
#define PPP 35
#define CCC (1000-PPP)

//pos_update_induc();
//position_update();
//pos_update_bemf();
//bemf_test


__IO uint16_t aADCTripleConvertedValue[150];
__IO uint16_t bADCTripleConvertedValue[150];

u32 last_ticks = 0;
u32 curr_ticks = 0;

u8 PWM_state = 0;

u8 FFFF = 0;
s16 ide_pos[DURATION]={0};
s16 mea_pos[DURATION]={0};
u32 adc_read[6][DURATION]={0,0,0,0,0,0};
u32 ind_read[3][DURATION]={0,0,0};
u8 method[DURATION]={0};
//u16 mea_x[6][2000]={0,0,0,0,0,0};


u32 hjk = 0;

void record(void);

void record_short(void);


void tri_FUCK(u8 input);

void tri_UNFUCK(void);


/*void DMA2_Stream0_IRQHandler(void) { //ADC DMA interrupt handler
	
	
	
	
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TEIF0 | DMA_IT_DMEIF0 | DMA_IT_FEIF0 | DMA_IT_TCIF0 | DMA_IT_HTIF0);


	tri_FUCK(0);
}*/


void test_algo(void){
	set_floating();
	//while(TIM3->CNT<10){}	
	//TEST_ALGO
	//_delay_us(50);
	TEST_ALGO
		
		
		
	//current_sensing_init();
	//cal_zero_mean();
	//PWM_init();
	//set_floating();
		
		//set_PWM(500,550,550);
	/*switch(PWM_state){
		case 0:{set_PWM(CCC,1001,1001);break;}
		case 1:{set_PWM(CCC,CCC,1001);break;}
		case 2:{set_PWM(1001,CCC,1001);break;}
		case 3:{set_PWM(1001,CCC,CCC);break;}
		case 4:{set_PWM(1001,1001,CCC);break;}
		case 5:{set_PWM(CCC,1001,CCC);break;}

	}	*/
		
	/*s16 eeerror = ABS(get_abs_angle()-get_pos()/2);
	
	if(eeerror>36){eeerror = 72-eeerror;}
	
	if(eeerror>10){
		uart_tx(COM3,"%d %d\n",get_abs_angle(),get_pos()/2);
	}*/
		
	 //set_floating();	
		//set_PWM(1001,1001,1001);
	
//	if(get_ticks()%2000>1000){
	set_PWM(0,0,0);
		//set_PWM((app_sin(((get_ticks())%100)*360)*PPP/32768)-500+CCC,(app_sin(((get_ticks()+33)%100)*360)*PPP/32768)+CCC-500,(app_sin(((get_ticks()-33)%100)*360)*PPP/32768)+CCC-500);
	//}else{
	  // set_PWM((app_sin(((get_ticks())%50)*720)*PPP/32768)-500+CCC,(app_sin(((get_ticks()+12)%50)*720)*PPP/32768)+CCC-500,(app_sin(((get_ticks()-12)%50)*720)*PPP/32768)+CCC-500);
//	}
		
    
	// set_PWM(app_sin(get_abs_angle()*500+9000)*PPP/32768-500,app_sin(get_abs_angle()*500+21000)*PPP/32768-500,app_sin(get_abs_angle()*500+33000)*PPP/32768-500);
	
}





s32 sum = 0;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	


	
	
	//ggpio_init();
	led_init();
	ticks_init();
	diode_init();
	HFI_init();
	BEMF_init();
	Scale_init();
	//dac_init(220,9);
	//DAC_enable_init();

	uart_init(COM3, 115200);
  btn_init();
	absEnc_init();
	diode_gnd(0); // 1 break
	HFI_read(0); // 1->read '
	BEMF_read(0);
	
	

	//_delay_ms(1000);
	//TIM5_init();

	//u16 last = 0;
//	bool now = 0;
	//current_sensing_init();
	sense_init();
	//current_sensing_init();

	//_delay_ms(1000);
	//cal_zero_mean();
	
	
	
	//DAC_enable(ALL_DISABLE);//	FET_gnd(FET_A);
	//current_sensing_init();
	//_delay_ms(1000);
  PWM_init();
	//_delay_ms(1000);
	//set_PWM(1001,1001,1001);
	//DAC_enable(ALL_DISABLE);
	//while(1);
	//uart_tx(COM3,"y=[");
	//while(1){}
			//test_algo();
		//	uart_tx(COM3,"];");
	while(1){	
		
		curr_ticks = get_ticks();


		
		
		
		/*for(u32 i=0;i<1000;i++){
			//test_algo();
			TEST_ALGO
			//while(!adc_done()){}

		}					
		uart_tx(COM3,"%d\n",get_ticks()-curr_ticks);
		while(1){}*/
		
			
			

		//_delay_us(1);
		if(last_ticks!=curr_ticks){
				last_ticks = curr_ticks;
			//	set_PWM(1001, 1001, 700);			
			//FET_gnd(FET_A);
				
			//DAC_enable(DAC_B);
			test_algo();
			//FET_gnd(FET_A);
		//	  set_floating();
			if(curr_ticks%100==1){
				//uart_tx(COM3,"%d,%d;",get_fake_abs_enc(),get_abs());
				//	TEST_ALGO
				//current_sensing_init();
			 	//uart_tx(COM3,"%d,",get_k());
				
				if(btn_onclick(SW)){
					//diode_gnd(sum%2);
					

					toggle_FUCK();
					
					//record_short();
					//record();
					//uart_bitch();
					/*if(FFFF==0){
						FET_gnd(NO_FET);
					}else{
						FET_gnd(1<<(FFFF-1));
					}*/
					
				}

				
				
				btn_update();
			}
			if(curr_ticks%50==3){
					PWM_state++;
					PWM_state%=6;

			}
			if(curr_ticks%1000==50){
				led_blink(LED_1);
			}
		}
	}



}


void record_short(void){
		
		for(u16 i=0;i<DURATION;){
			curr_ticks = get_ticks();
			if(curr_ticks!=last_ticks){
				last_ticks = curr_ticks;
	  		test_algo();
			//	uart_tx(COM3,"%d,%d;",get_pos(),(get_abs()%73));
				ide_pos[i]=get_abs_angle();
				mea_pos[i]=get_pos()/2;
				
				/*adc_read[0][i]=get_induc(0);
				adc_read[1][i]=get_induc(1);
				adc_read[2][i]=get_induc(2);*/
				
				
				/*adc_read[0][i]=get_bemf_read(0);
				adc_read[1][i]=get_bemf_read(1);
				adc_read[2][i]=get_bemf_read(2);*/

				
				adc_read[0][i]=get_read(0);
				adc_read[1][i]=get_read(1);
				adc_read[2][i]=get_read(2);
				

				if(get_method()){
					method[i]=75;
				}else{
					method[i]=0;
				}
				i++;

		}
					
		}
		uart_tx(COM3,"y=[");
		for(u16 i=0;i<DURATION/2;i++){
		  //uart_tx(COM3,"%d,%d,%d,%d,%d,%d;",adc_read[0][i],adc_read[1][i],adc_read[2][i],adc_read[3][i],adc_read[4][i],adc_read[5][i]);
		  uart_tx(COM3,"%d,%d,%d;",mea_pos[i],ide_pos[i],method[i]);
			//uart_tx(COM3,"%d,%d,%d;",adc_read[0][i],adc_read[1][i],adc_read[2][i]);
			_delay_ms(5);
		}

		uart_tx(COM3,"];plot(y);ylim([-5 75])");
		//uart_tx(COM3,"];plot(y)");
	}



void record(void){
		uart_tx(COM3,"y=[");
		for(u16 i=0;i<DURATION;){
			curr_ticks = get_ticks();
			if(curr_ticks!=last_ticks){
				last_ticks = curr_ticks;
	  		//TEST_ALGO
				test_algo();
				current_sensing_init(); 
			//	uart_tx(COM3,"%d,%d;",get_pos(),(get_abs()%73));
				ide_pos[i]=((get_abs()+93)%146);
				mea_pos[i]=get_pos();
				
				adc_read[0][i]=get_read(0);
				adc_read[1][i]=get_read(1);
				adc_read[2][i]=get_read(2);
				
				ind_read[0][i]=get_induc(0);
				ind_read[1][i]=get_induc(1);
				ind_read[2][i]=get_induc(2);
				if(get_method()){
					method[i]=145;
				}else{
					method[i]=0;
				}
				i++;
		}
					
		}
		
		for(u16 i=0;i<DURATION;i++){
			//uart_tx(COM3,"%d %d %d;",mea_pos[i],ide_pos[i],method[i]);
			uart_tx(COM3,"%d,%d,%d,",mea_pos[i],ide_pos[i],method[i]);
			_delay_ms(5);
			uart_tx(COM3,"%d,%d,%d,",adc_read[0][i],adc_read[1][i],adc_read[2][i]);
			_delay_ms(5);
			uart_tx(COM3,"%d,%d,%d;",ind_read[0][i],ind_read[1][i],ind_read[2][i]);
			_delay_ms(5);
		}

		uart_tx(COM3,"];plot(y(1:%d,1:3));",DURATION);
		_delay_ms(5);
		uart_tx(COM3,"ylim([-5 150]);figure;");
		_delay_ms(5);
		uart_tx(COM3,"plot(y(1:%d,4:6));",DURATION);
		_delay_ms(5);
		uart_tx(COM3,"figure;plot(y(1:%d,7:9));",DURATION);


}




u8 cc = 0;

void tri_FUCK(u8 input){
	
	ADC_DeInit();
	DMA_DeInit(DMA2_Stream0);
	
	
  GPIO_InitTypeDef       GPIO_InitStructure;
  DMA_InitTypeDef        DMA_InitStructure;
  ADC_InitTypeDef        ADC_InitStructure;
  ADC_CommonInitTypeDef  ADC_CommonInitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period =1;        //100 => 13k 20=>62.5k 30=>42.37k  10=>120k      9=>130
  TIM_TimeBaseStructure.TIM_Prescaler = 17;                                                               
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;                                                   
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_DeInit(TIM8);
     
// TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);

  TIM_Cmd(TIM8, ENABLE);	
	
	
  
  /* Enable peripheral clocks *************************************************/
	
	
	if(cc == 0){
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2 , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2 , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC3 , ENABLE);  	
		
	


  /* Configure ADC Channel 12 pin as analog input *****************************/ 
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

  /* DMA2 Stream0 channel0 configuration **************************************/
	
	
	
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)0x40012308);
	if(cc == 0){
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&aADCTripleConvertedValue;
		cc++;
	}else{
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&bADCTripleConvertedValue;
	}
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 150;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_STREAMx, &DMA_InitStructure);

  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* ADC Common configuration *************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_Interl;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_7Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;  
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; 
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 regular channel 12 configuration ************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
	
//	ADC_DeInit();
	
  ADC_Init(ADC1, &ADC_InitStructure);


	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	
	
	//ADC_DeInit();


  ADC_RegularChannelConfig(ADC1, input, 1, ADC_SampleTime_3Cycles);
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* ADC2 regular channel 12 configuration ************************************/
  ADC_Init(ADC2, &ADC_InitStructure);
  /* ADC2 regular channel12 configuration */ 
  ADC_RegularChannelConfig(ADC2, input, 1, ADC_SampleTime_3Cycles);

  /* ADC3 regular channel 12 configuration ************************************/
  ADC_Init(ADC3, &ADC_InitStructure); 
  /* ADC3 regular channel12 configuration */
  ADC_RegularChannelConfig(ADC3, input, 1, ADC_SampleTime_3Cycles);

  /* Enable DMA request after last transfer (multi-ADC mode) ******************/
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC2 **************************************************************/
  ADC_Cmd(ADC2, ENABLE);

  /* Enable ADC3 **************************************************************/
  ADC_Cmd(ADC3, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 	NVIC_Init(&NVIC_InitStructure); 

	
	ADC_SoftwareStartConv(ADC1);
	
	
	
	

}


// Fucking DMA_DeInit(), I've spent nearly 3 day to solve this  


void tri_UNFUCK(void){
	
	//	NVIC_InitTypeDef NVIC_InitStructure;
	 // ADC_CommonInitTypeDef  ADC_CommonInitStructure; 
	
	  //ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
		//ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_7Cycles;
		//ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;  
		//ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; 
		//ADC_CommonInit(&ADC_CommonInitStructure);
	
		//RCC_AHB1PeriphClockCmd( ADC1_2_CHANNEL_GPIO_CLK , DISABLE);
	
		ADC_DeInit();
		DMA_DeInit(DMA2_Stream0);
	
		//RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA2 , DISABLE);
		//RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 , DISABLE);
		//RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2 , DISABLE);
		//RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC3 , DISABLE); 

	
		//DMA_Cmd(DMA_STREAMx, DISABLE);
		
		
		//DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, DISABLE);
		//ADC_DMACmd(ADC1, DISABLE);
		
		
		//ADC_Cmd(ADC1, DISABLE);
		//ADC_Cmd(ADC2, DISABLE);	
		//ADC_Cmd(ADC3, DISABLE);
		
		
		//ADC1->CR2 &= (uint32_t)~ADC_CR2_SWSTART;
		//ADC_MultiModeDMARequestAfterLastTransferCmd(DISABLE);
	
		//NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
		//NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		//NVIC_Init(&NVIC_InitStructure); 


}
