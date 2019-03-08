#include "main.h"

#define DELAY 10
#define TEST_ALGO position_update();
#define DURATION 1000
//pos_update4();
//position_update();
//pos_update_bemf();
u32 last_ticks = 0;
u32 curr_ticks = 0;


u8 FFFF = 0;
s16 ide_pos[DURATION]={0};
s16 mea_pos[DURATION]={0};
u32 adc_read[3][DURATION]={0,0,0};
u32 ind_read[3][DURATION]={0,0,0};
u8 method[DURATION]={0};
//u16 mea_x[6][2000]={0,0,0,0,0,0};

void record(void);

void record_short(void);

void ggpio_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIOC->ODR |= 0x3800;
	
	
}

s32 sum = 0;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	ggpio_init();
	led_init();
	ticks_init();
	//DAC_enable_init();
	uart_init(COM3, 115200);
  btn_init();
	absEnc_init();
	//TIM5_init();

	//u16 last = 0;
//	bool now = 0;
	//current_sensing_init();
	//current_sensing_init();
	sense_init();	
	current_sensing_init();
	//DAC_enable(ALL_DISABLE);
	//while(1);
	//uart_tx(COM3,"y=[");
				
			
		//	uart_tx(COM3,"];");
	while(1){	
		
		curr_ticks = get_ticks();


		
		
		
	/*	for(u32 i=0;i<100000;i++){
			//inv_sqrt(1);
				TEST_ALGO
				current_sensing_init();
			//reset_dma_adc(SENSE_C);
			//while(!adc_done()){}

		}					
		uart_tx(COM3,"%d\n",get_ticks()-curr_ticks);
		while(1){}*/
		
			
			

		//_delay_us(1);
		if(last_ticks!=curr_ticks){
				last_ticks = curr_ticks;	
			  //TEST_ALGO
				//current_sensing_init(); 
			if(curr_ticks%100==1){

				//	TEST_ALGO
				//current_sensing_init();
			 	//6uart_tx(COM3,"%d\n",get_abs());
				
				if(btn_onclick(SW)){

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
			if(curr_ticks%10==3){


			}
			if(curr_ticks%1000==50){
				led_blink(LED_1);
			}
		}
	}



}


void record_short(void){
		uart_tx(COM3,"y=[");
		for(u16 i=0;i<DURATION/2;){
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
		
		for(u16 i=0;i<DURATION/2;i++){
			//uart_tx(COM3,"%d %d %d;",mea_pos[i],ide_pos[i],method[i]);
			uart_tx(COM3,"%d,%d,%d;",mea_pos[i],ide_pos[i],method[i]);
			_delay_ms(5);
		}

		uart_tx(COM3,"];plot(y);ylim([-5 150])");
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
