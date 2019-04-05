#include "main.h"

#define DELAY 10
#define TEST_ALGO pos_update_induc();
#define DURATION 1000
#define PPP 25
#define CCC (1000-PPP)

//pos_update_induc();
//position_update();
//pos_update_bemf();
//bemf_test
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

void record(void);

void record_short(void);



void test_algo(void){
	set_floating();
 
	while(TIM3->CNT<10){}	

  _delay_us(40);		
	TEST_ALGO

	current_sensing_init();
	//cal_zero_mean();
	PWM_init();
		
		
		//set_PWM(500,550,550);
	/*switch(PWM_state){
		case 0:{set_PWM(CCC,1001,1001);break;}
		case 1:{set_PWM(CCC,CCC,1001);break;}
		case 2:{set_PWM(1001,CCC,1001);break;}
		case 3:{set_PWM(1001,CCC,CCC);break;}
		case 4:{set_PWM(1001,1001,CCC);break;}
		case 5:{set_PWM(CCC,1001,CCC);break;}

	}	*/
		
	set_floating();	
		

// set_PWM((app_sin(((get_ticks())%100)*360)*PPP/32768)+CCC,(app_sin(((get_ticks()+33)%100)*360)*PPP/32768)+CCC,(app_sin(((get_ticks()-33)%100)*360)*PPP/32768)+CCC);

	
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
	current_sensing_init();

	_delay_ms(1000);
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


		
		
		
		for(u32 i=0;i<1000;i++){
			test_algo();
			//TEST_ALGO
			//while(!adc_done()){}

		}					
		uart_tx(COM3,"%d\n",get_ticks()-curr_ticks);
		while(1){}
		
			
			

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
				
				//	TEST_ALGO
				//current_sensing_init();
			 	//uart_tx(COM3,"%d,",get_abs());
				
				if(btn_onclick(SW)){
					//diode_gnd(sum%2);
					

					//HFI_read(sum%2);
					record_short();
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
				
			 if(curr_ticks%100==3){
					PWM_state++;
					PWM_state%=6;

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
