#include "main.h"


u32 last_ticks = 0;
u32 curr_ticks = 0;


u32 time_t[2000]={0};
s16 ide_pos[2000]={0};
s16 mea_pos[2000]={0};
u16 mea_x[6][2000]={0,0,0,0,0,0};

void record(void);


int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	

	led_init();
	ticks_init();
	DAC_enable_init();
	
	dac_init(150,3);
	uart_init(COM1, 115200);
	adc_init();
  btn_init();
	absEnc_init();
	//_delay_ms(10);
	TIM5_init();
	
	_delay_ms(100);
	//u16 last = 0;
	u16 count = 0;
	u16 count1 = 0;
	u16 now = 0;
		
	//DAC_enable(DAC_A);
	//FET_gnd(FET_B);
	//DAC_enable(DAC_A);
	//FET_gnd(FET_B);
	//while(1);
//	uart_tx(COM1,"y=[");

	while(1){	
		curr_ticks = get_ticks();
		
		/*reset_dma_adc(SENSE_C);
	  while(!adc_done()){}
		uart_reading();
		while(1){}
		
		curr_ticks = get_ticks();
	

		for(u32 i=0;i<1000;i++){
			//pos_sense();
		//	pos_update();
			//reset_dma_adc(SENSE_C);
			//while(!adc_done()){}

		}					
		uart_tx(COM1,"%d\n",get_ticks()-curr_ticks);
		while(1){}*/
		
			
			
		if(curr_ticks>1){ // !!!! ESSENTIAL CODE !!!!  
	    
			//reset_dma_adc(SENSE_B);
		}
		//_delay_us(1);
		if(last_ticks!=curr_ticks){
			pos_update2();
		//	uart_tx(COM1,"%d,%d;",get_pos(),(get_abs()%73));
			last_ticks = curr_ticks;
			if(curr_ticks%10==1){

//				reset_dma_adc(SENSE_C);
				//bitch[get_abs()]=get_pk2pk();					

				
				if(btn_onclick(SW_2)){

					uart_bitch();
				}
				if(btn_onclick(SW_3)){

				}
				
				
				btn_update();
			}
			if(curr_ticks%10==3){

				//uart_tx(COM1,"%d,",get_pos());//get_pk2pk(SENSE_C));
				 //uart_tx(COM1,"%d\n",get_abs());
				//uart_tx(COM1,"%d %d %d\n",get_pk2pk(),get_count(),curr_ticks);
			}
			if(curr_ticks%1000==50){
				led_blink(LED_1);
				//uart_tx(COM1,"%d %d %d\n",get_pk2pk(),get_count(),curr_ticks);
			}
		}
	}

		/*if(flag){
		for(u16 j=2;j<sense_buffer;j++){
		 uart_tx(COM1,"%d,",on9ADC[j]);
			_delay_ms(10);
		}
		uart_tx(COM1,"\n");
		uart_tx(COM1,"%d;",pk2pk());
		flag = 0;
		}*/
		/*if(last!=get_abs()){
			//uart_tx(COM1,"%d,%d;\n",gray2bin(~GPIOE->IDR&0x03FF),pk2pk());
			
			last = get_abs();
			//uart_tx(COM1,"%d\n",get_abs());
			uart_tx(COM1,"%d ",pk2pk());
			//uart_tx(COM1,"%d\n",get_ticks());
			
			//uart_binary((~(GPIOE->IDR))&0x3FF,10);
		}*/
		

	//if(btn_pressed(BUTTON_1)){
		/*for(u16 j=2;j<sense_buffer;j++){
		 uart_tx(COM1,"%d,",on9ADC[j]);
			_delay_ms(10);
		}
		uart_tx(COM1,";%d,",pk2pk());
			reset_dma_adc();*/
		//for(u16 i=0;i<1024;i++){
		//	if(pk_map[i]==0){
		//		uart_tx(COM1,"%d,",i);
		//		_delay_ms(10);
		//	}
		//}
		
	//}	
	//if(btn_pressed(BUTTON_2)){
			//uart_binary(GPIOE->IDR,10);
		  //uart_binary(gray2bin(GPIOE->IDR&0x03FF),10);
		//	for(u16 i=0;i<1024;i++){
		//		uart_tx(COM1,"%d,",pk_map[i]);
		//		_delay_ms(5);
		//	}
		//	uart_tx(COM1,";");
			//_delay_ms(400);
//	}	
//	}	
	
	/*u16 i=100;
	while(1){
			for(u8 j=0;j<7;j++){
				dac_init(i,j);
				_delay_ms(2000);
			}
			i+=10;
	}*/
	

}


void record(void){
		uart_tx(COM1,"y=[");
		for(u16 i=0;i<2000;){
			curr_ticks = get_ticks();
			if(curr_ticks!=last_ticks){
				last_ticks = curr_ticks;
				time_t[i]=curr_ticks;
	  		pos_update2();
			//	uart_tx(COM1,"%d,%d;",get_pos(),(get_abs()%73));
				ide_pos[i]=(get_abs()%73);
				mea_pos[i]=get_pos();
				
				i++;
		}
					
		}
		uart_tx(COM1,"];");


}
