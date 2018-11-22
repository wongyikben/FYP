#include "main.h"


u32 last_ticks = 0;
u32 curr_ticks = 0;





int main(void) {
	SystemInit();
	SystemCoreClockUpdate();

	led_init();
	ticks_init();
	DAC_enable_init();
	dac_init(150,3);
	timer_init();
	uart_init(COM1, 115200);
	adc_init();
  btn_init();
	absEnc_init();
	//_delay_ms(10);
	//u16 last = 0;
	u16 count = 0;
	u16 count1 = 0;
	u16 now = 0;
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	
	while(1){	
		
		curr_ticks = get_ticks();

/*		for(u32 i=0;i<10000;i++){
			pos_update();
		}					
		uart_tx(COM1,"%d",get_ticks()-curr_ticks);
		while(1){}*/
		
			
			
		if(curr_ticks>1){ // !!!! ESSENTIAL CODE !!!!  
		  pos_update();
			//reset_dma_adc(SENSE_B);
		}
		//_delay_us(1);
		if(last_ticks!=curr_ticks){
			last_ticks = curr_ticks;
			if(curr_ticks%10==1){
				//get_pk2pk();
				//bitch[get_abs()]=get_pk2pk();					

				
				if(btn_onclick(SW_2)){

					uart_bitch();
				}
				if(btn_onclick(SW_3)){
					//DAC_enable((count%3));
					//reset_dma_adc(0);
					//count++;
					uart_reading();
		
		


				
				}
				
				
				btn_update();
			}
			if(curr_ticks==100){
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
