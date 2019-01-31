#include "main.h"

#define DELAY 10
#define TEST_ALGO position_update();
#define DURATION 500
//pos_update4();
//position_update();
//pos_update_bemf();
u32 last_ticks = 0;
u32 curr_ticks = 0;

u8 FFFF = 0;
s16 ide_pos[DURATION]={0};
s16 mea_pos[DURATION]={0};
u8 method[DURATION]={0};
u16 mea_x[6][2000]={0,0,0,0,0,0};

void record(void);


int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	

	led_init();
	ticks_init();
	DAC_enable_init();
	dac_init(150,3);
	//uart_init(COM3, 115200);
  btn_init();
	//absEnc_init();
	//TIM5_init();
	
	//_delay_ms(100);
	//u16 last = 0;
	bool now = 0;
	//sense_init();	
	//DAC_enable(DAC_A);
	//FET_gnd(FET_B);
	//DAC_enable(DAC_A);
	//while(1);
	//uart_tx(COM3,"y=[");

	while(1){	
		
		curr_ticks = get_ticks();
	



		
		
		
		/*for(u32 i=0;i<1000;i++){
			//inv_sqrt(1);
			TEST_ALGO
			//reset_dma_adc(SENSE_C);
			//while(!adc_done()){}

		}					
		uart_tx(COM3,"%d\n",get_ticks()-curr_ticks);
		while(1){}*/
		
			
			

		//_delay_us(1);
		if(last_ticks!=curr_ticks){

		//	TEST_ALGO
			
		
		//	uart_tx(COM3,"%d,%d;",get_pos(),(get_abs()%73));
			last_ticks = curr_ticks;
		
			
			if(curr_ticks%10==1){

					
				//uart_tx(COM3,"%d\n",get_abs());
				
				if(btn_onclick(SW)){
					FFFF++;
					FFFF%=4;
					DAC_enable(ALL_DISABLE);
					
					
					
					/*if(FFFF==0){
						FET_gnd(NO_FET);
					}else{
						FET_gnd(1<<(FFFF-1));
					}*/
					
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
		uart_tx(COM3,"y=[");
		for(u16 i=0;i<DURATION;){
			curr_ticks = get_ticks();
			if(curr_ticks!=last_ticks){
				last_ticks = curr_ticks;
	  		TEST_ALGO
			//	uart_tx(COM3,"%d,%d;",get_pos(),(get_abs()%73));
				ide_pos[i]=((get_abs()+93)%146);
				mea_pos[i]=get_pos();
				if(get_method()){
					method[i]=145;
				}else{
					method[i]=0;
				}
				i++;
		}
					
		}
		
		for(u16 i=0;i<DURATION;i++){
			uart_tx(COM3,"%d %d %d;",mea_pos[i],ide_pos[i],method[i]);
			_delay_ms(5);
		}

		uart_tx(COM3,"];plot(y);ylim([-5 150])");
		


}
