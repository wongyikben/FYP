#include "main.h"

#define DELAY 10
#define TEST_ALGO position_update();
//pos_update_bemf();
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
	bool now = 0;
	mat_init();	
	//DAC_enable(DAC_A);
	//FET_gnd(FET_B);
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	//while(1);
	//uart_tx(COM1,"y=[");

	while(1){	
		
		curr_ticks = get_ticks();
	



		
		
		
	/*	for(u32 i=0;i<1000;i++){
			//inv_sqrt(1);
			TEST_ALGO
			//reset_dma_adc(SENSE_C);
			//while(!adc_done()){}

		}					
		uart_tx(COM1,"%d\n",get_ticks()-curr_ticks);
		while(1){}*/
		
			
			
		if(curr_ticks>10000){ // !!!! ESSENTIAL CODE !!!!  
	   // uart_tx(COM1,"];");
			//while(1){}
			//reset_dma_adc(SENSE_B);
		}
		//_delay_us(1);
		if(last_ticks!=curr_ticks){
/*	FET_gnd(FET_B);
	//DAC_enable(DAC_A);
	//_delay_us(DELAY);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	//DAC_enable(DAC_B);
	FET_gnd(FET_C);
	//_delay_us(DELAY);
	reset_dma_adc(SENSE_A);
	
	while(!adc_done()){}
	//DAC_enable(DAC_C);
	FET_gnd(FET_A);
	//delay_us(DELAY);		
	reset_dma_adc(SENSE_B);

	while(!adc_done()){}		
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);*/
			
			TEST_ALGO
			
		//	uart_tx(COM1,"%d\n",get_vel());
		//	uart_tx(COM1,"%d,%d;",get_pos(),(get_abs()%73));
			last_ticks = curr_ticks;
			
			if(now!=get_method()){
				now=get_method();
				if(now){
				uart_tx(COM1,"HFIM");}
				else{
				uart_tx(COM1,"BEMF");
				}
			
			}
			
			
			if(curr_ticks%10==1){
					
				//uart_tx(COM1,"%d\n",get_abs());
				
				if(btn_onclick(SW_2)){
					record();
					//uart_bitch();
				}
				if(btn_onclick(SW_3)){
					uart_bitch();	
				}
				
				
				btn_update();
			}
			if(curr_ticks%10==3){


			}
			if(curr_ticks%1000==50){
				led_blink(LED_1);
			}
		}
	}



}


void record(void){
		uart_tx(COM1,"y=[");
		for(u16 i=0;i<500;){
			curr_ticks = get_ticks();
			if(curr_ticks!=last_ticks){
				last_ticks = curr_ticks;
				time_t[i]=curr_ticks;
	  		TEST_ALGO
			//	uart_tx(COM1,"%d,%d;",get_pos(),(get_abs()%73));
				ide_pos[i]=((get_abs()+93)%73);
				mea_pos[i]=get_pos();
				
				i++;
		}
					
		}
		
		for(u16 i=0;i<500;i++){
			uart_tx(COM1,"%d %d;",mea_pos[i],ide_pos[i]);
			_delay_ms(5);
		}

		uart_tx(COM1,"];");


}
