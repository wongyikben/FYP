#include "../core/pos_esti.h"
#include "macro_math.h"
#include "../core/vec.h"
#include "approx_math.h"
#include "../CurrentSensing.h"
#include "adc.h"
#include "../dac.h"
#include "../absEnc.h"
#include "trigon_math.h"
#include "uart.h"
#include "../PWM.h"

#define DELAY 7
#define BEMF_DELAY 1
#define K_THRESH 100000
#define k_thresh 5000


s32 position = 0;
s32 last_deg = 0;
s32 last_position = 0;
s32 curr_position = 0;

s32 last_pos = 0;
s32 enc = 0;
s32 last_enc = 0;
s32 enc_vel = 0;
u16 fake_abs_enc = 512;



u8 true_count = 0;
s32 x[3] = {0};



s32 result[3] = {0};
u32 k = 0;
s32 ksin = 0;
s32 kcos = 0;
s32 offset = 0;

s16 bemf_read[6]={0};

vec3 x_vec[2];
vec3 read;
vec3 last_read;


u8 trash = 0;

s32 WTF[3][73] = {0};

u16 init_pos = 0;

bool method_flag = false;



bool sense_method = true;



s32 output[3];
mat3 J[2];
vec3 x_temp[2];

s32 bitch [1024] = {0};
s32 bitch1[3][1024] = {0};

s16 bitch_index = 0;

s16 last = 0;
s16 curr = 0;
s16 next = 0;
s16 vel = 0;
s16 error = 0;


s32 get_pos(void){
	return position;

}





void v_funct(vec3 input){

s32 v = get_voltage();

s32 a = input.n[0];
s32 b = input.n[1];	
s32 c = input.n[2];	

s32 R = 12412;

output[0] = ((v*b)*inv_sqrt(R*R+(a+b)*(a+b)))>>9;
output[1] = ((v*c)*inv_sqrt(R*R+(c+b)*(c+b)))>>9;
output[2] = ((v*a)*inv_sqrt(R*R+(a+c)*(a+c)))>>9;

}

u16 get_fake_abs_enc(void){
	return fake_abs_enc/2;
}
void enc_cal(void){
	
	if(ABS(position*true_count-last_pos)>72){
		if(position*true_count-last_pos<0){
			enc+= (position*true_count-last_pos+144);
		}else{
			enc+= (position*true_count-last_pos-144);
		}
	
	}else{
			enc += (position*true_count-last_pos);
	}
	last_pos = position;
	
	
	// velocity 
	enc_vel = (enc-last_enc);
	fake_abs_enc-=enc_vel;
	if(fake_abs_enc<0){fake_abs_enc+=2048;}
	fake_abs_enc%=2048;
	last_enc=enc;

}

void induc_sense(void){

		
	//diode_gnd(1); // 1 break
	//HFI_read(1); // 1->read '
	
	//_delay_us(80);
	
	/*DAC_enable(DAC_C);
	FET_gnd(FET_A);
	_delay_us(DELAY);	
	reset_dma_adc(SENSE_B,0);
	while(!adc_done()){}*/
		
	

		
		
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	_delay_us(DELAY);
  reset_dma_adc(SENSE_C,1);
	while(!adc_done()){}

	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	_delay_us(DELAY);	
	reset_dma_adc(SENSE_A,1);
	read.n[0] = (s32)(get_pk2pk(SENSE_C))<<7;

	while(!adc_done()){}
	//FET_gnd(NO_FET);
	//_delay_us(DELAY);	
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	_delay_us(DELAY);	
	reset_dma_adc(SENSE_B,1);
	read.n[1] = (s32)(get_pk2pk(SENSE_A))<<7;
		

	
	while(!adc_done()){}
	read.n[2] = (s32)(get_pk2pk(SENSE_B))<<7;			
	DAC_enable(ALL_DISABLE);
	FET_gnd(NO_FET);
		
	diode_gnd(0); // 1 break
	HFI_read(0); // 1->read '
	
}



void bemf_sense(void){
	BEMF_read(1);
	DAC_enable(ALL_DISABLE);
	//FET_gnd(NO_FET);
	


 	//x[0] = (s32)(get_bemf(SENSE_A))<<7;
	//bemf_read[0]=x[0]>>7;
	
	reset_dma_adc_bemf(SENSE_C);
	while(!adc_done()){}
	x[2] = (s32)(get_bemf(SENSE_C))<<7;
	bemf_read[2]=x[2]>>7;

	reset_dma_adc_bemf(SENSE_B);	
	while(!adc_done()){}
	x[1] = (s32)(get_bemf(SENSE_B))<<7;
	bemf_read[1]=x[1]>>7;
		
	reset_dma_adc_bemf(SENSE_A);	
	while(!adc_done()){}
	x[0] = (s32)(get_bemf(SENSE_A))<<7;
	bemf_read[0]=x[0]>>7;
	BEMF_read(0);
		
		
		
}



void N_method(){
	
	vec3 offset;
	
	
	vec3 temp;
	
	v_funct(x_vec[0]);
	
	temp.n[0]=output[0];
	temp.n[1]=output[1];
	temp.n[2]=output[2];
	
	x_vec[0].n[0] = x_vec[0].n[0]+1;
	v_funct(x_vec[0]);
	J[0].n[0]=output[0]-temp.n[0]; // I didn't /the delta because it is 1 
	J[0].n[3]=output[1]-temp.n[1];
	J[0].n[6]=output[2]-temp.n[2];
	
	x_vec[0].n[0] = x_vec[0].n[0]-1;
	x_vec[0].n[1] = x_vec[0].n[1]+1;
	v_funct(x_vec[0]);
	J[0].n[1]=output[0]-temp.n[0]; 
	J[0].n[4]=output[1]-temp.n[1];
	J[0].n[7]=output[2]-temp.n[2];
	
	x_vec[0].n[1] = x_vec[0].n[1]-1;
	x_vec[0].n[2] = x_vec[0].n[2]+1;
	v_funct(x_vec[0]);
	J[0].n[2]=output[0]-temp.n[0]; 
	J[0].n[5]=output[1]-temp.n[1];
	J[0].n[8]=output[2]-temp.n[2];
	
	x_vec[0].n[2] = x_vec[0].n[2]-1;
	J[0]=inverse(J[0]);
	

	offset = mat_vec_prod(J[0],vec_sub(temp,read));
	sca_vec_div(128,&offset);
	x_vec[0]=vec_sub(x_vec[0],offset);
}

void sense_init(void){

	dac_init(220,8);
	DAC_enable_init();
	adc_init();
	//FET_GPIO_init();
	//vec3 temp;

	DAC_enable(ALL_DISABLE);
	
	//GPIOC->ODR &= ~(0x0038);
	

	
	last_deg = get_abs_angle()*2;
	
	
	x_vec[0].n[0]=1;
	x_vec[0].n[1]=1;
	x_vec[0].n[2]=1;
	
	induc_sense();

	
	

	for (u8 i=0;i<5;i++){	
		N_method();
	}
	
  pos_update_induc();
	
	
	
}

void bemf_test(void){

	bemf_sense();
	
	offset = (x[0]+x[1]+x[2])/3;

	x[0]-=offset;
	x[1]-=offset;
	x[2]-=offset;	
	

	ksin = (x[0]-x[1]-x[2])>>9;
	kcos = (x[1]-x[2])/443;
	position = (-app_atan2(ksin,kcos));
	position/=250;
	k = (((ksin*ksin+kcos*kcos))*25+k*75)/100;
	if(position<0){position+=144;}
	position%=72;
			
 	s16 error_under=ABS(last_deg-position);
	s16 error_upper=ABS(last_deg-(72+position));
	if(error_under>72){error_under = 144-error_under;}
	if(error_upper>72){error_upper = 144-error_upper;}
	if(error_upper<error_under){position+=72;}	
			
	if(position<0){position+=144;}
	position%=144;

	enc_cal();
	last_deg = position;
	last_position = position;			
}





void pos_update_induc(void){
		
	//bemf_sense();

		
	s32 tri_x[3]={0};

	diode_gnd(1); // 1 break
	HFI_read(1); // 1->read 
	FET_gnd(NO_FET);
	induc_sense();
	

	u16 temp = get_abs();
		
	
	//N_method();
	//N_method();
	//N_method();
	

	
	// Position estimation code 


	

	
	tri_x[0] = read.n[2];//(x_vec[0].n[0]-937)<<7;
	tri_x[1] = read.n[0];//(x_vec[0].n[1]-905)<<7;
	tri_x[2] = read.n[1];//(x_vec[0].n[2]-954)<<7;
	

	
	offset = (tri_x[0]+tri_x[1]+tri_x[2])/3;

	tri_x[0]-=offset;
	tri_x[1]-=offset;
	tri_x[2]-=offset;
	
	
	

	ksin = (tri_x[0]-tri_x[1]-tri_x[2])>>8; //2^9 = 2*256
	kcos = (tri_x[1]-tri_x[2])/222;   //443 = 2sin(pi/3)*256
	position = app_atan2(ksin,kcos);


	position = (position+9000)/500;
	position%=72;
	last_position%=72;

	
	
	
	// mean filter 
/*if(true_count){	
		if(ABS(position-last_position)>36){
		if(position>last_position){
			last_position+=71;
			curr_position = ((position+last_position)>>1)%72;
			last_position = position;
		}else{
			position+=71;
			curr_position = ((position+last_position)>>1)%72;
			last_position = position-71;
		}
	
	}else{
		curr_position = ((position+last_position)>>1)%72;
		
		last_position = position;
	}
	position = curr_position;
	
}
*/


	position-=9;//????  I KNOW WHY LA Because this method did not seperate Ld Lq, so this offset is the cos(Ld/Lq) 
	if(position<0){position+=71;}
	position%=72;
	
	


		//uart_tx(COM3,"%d %d %d\n",read.n[0]>>7,read.n[1]>>7,read.n[2]>>7);
	
	// solve polarity ambiguity 	
		s16 error_under=ABS(last_deg-position);
		s16 error_upper=ABS(last_deg-(72+position));
		if(error_under>72){error_under = 144-error_under;}
		if(error_upper>72){error_upper = 144-error_upper;}
		if(error_upper<error_under){position+=72;}		
	
	
	last_deg = position;

	
	// translate to encoder 
	enc_cal();
	
	
	// error calculation 
	//position = (position*146/144);
	true_count=1;
	current_sensing_init();
	PWM_init();	
	
	diode_gnd(0); // 1 break
	HFI_read(0); // 1->read '
	
		
}




void position_update(void){
	_delay_us(90);	
	bemf_sense();		
	offset = (x[0]+x[1]+x[2])/3;

	x[0]-=offset;
	x[1]-=offset;
	x[2]-=offset;
	

	ksin = (x[0]-x[1]-x[2])>>9;
	kcos = (x[1]-x[2])/443;
	
	k = (((ksin*ksin+kcos*kcos))*20+k*80)/100;//>>1)+(k>>1);


//	uart_tx(COM3,"%d\n",k);
	
	//if((k>K_THRESH&&sense_method)||(k>k_thresh&&!sense_method)){		
		if(k>K_THRESH){
			position = (-app_atan2(ksin,kcos));
			position/=250;
	
			if(position<0){position+=144;}
			position%=72;
			
			s16 error_under=ABS(last_deg-position);
			s16 error_upper=ABS(last_deg-(72+position));
			if(error_under>72){error_under = 144-error_under;}
			if(error_upper>72){error_upper = 144-error_upper;}
			if((error_upper<error_under)&&position<72){position+=72;}	
			//if((error_upper>error_under)&&position>72){position-=72;}	
			
		  if(position<0){position+=144;}
			position%=144;

			enc_cal();
			last_deg = position;
			last_position = position;
			true_count=1;
			sense_method=false;
			return;
	}
	if(!sense_method){
		reset_filter_count();
		induc_sense();
	}
	pos_update_induc();
	sense_method=true;
}



void uart_bitch(void){
	uart_tx(COM3,"y=[");		
	for(u16 i=0;i<73;i++){
		uart_tx(COM3,"%d,",WTF[0][i]);
		uart_tx(COM3,"%d,",WTF[1][i]);
		uart_tx(COM3,"%d;",WTF[2][i]);
//	uart_tx(COM3,"%d,",bitch1[1][i]);
//	uart_tx(COM3,"%d;",bitch1[2][i]);
		_delay_ms(10);
	}
	
/*	x_offset[0] = (max[0]+min[0])>>1;
	x_offset[1] = (max[1]+min[1])>>1;
	x_offset[2] = (max[2]+min[2])>>1;
	x_offset[3] = (max[3]+min[3])>>1;
	x_offset[4] = (max[4]+min[4])>>1;
	x_offset[5] = (max[5]+min[5])>>1;*/
	
	//uart_tx(COM3,"%d,\n",bitch_index);
/*	uart_tx(COM3,"%d,",x_offset[0]>>7);
	uart_tx(COM3,"%d,",x_offset[1]>>7);
	uart_tx(COM3,"%d,",x_offset[2]>>7);
	uart_tx(COM3,"%d,",x_offset[3]>>7);
	uart_tx(COM3,"%d,",x_offset[4]>>7);
	uart_tx(COM3,"%d,",x_offset[5]>>7);*/
	
	uart_tx(COM3,"];\n");
}


void TIM5_init(void) {
	
	init_pos = get_abs();
	
	TIM_DeInit(TIM5);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
   
	//Setup timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_Prescaler = 85;
  TIM_TimeBaseStructure.TIM_Period = 1000;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
	//Setup interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	


}


u32 get_k(void){
	return k;
}

void TIM5_IRQHandler(void){
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
/*	curr = get_abs();
	vel = curr - last;
	if(vel>512){vel-=1024;}
	if(vel<-512){vel+=1024;}
	error = curr-next;
	if(error>512){error-=1024;}
	if(error<-512){error+=1024;}
	uart_tx(COM3,"%d,",error);
	last = curr;
	next = (curr+vel)%1024;*/

	
}

s32 get_vel(void){
	return enc_vel;
}


s32 get_enc(void){
	return enc;
}




u32 get_read(u8 index){
	return read.n[index]>>6;
}

u32 get_induc(u8 index){
	return x_vec[0].n[index];
}

s16 get_bemf_read(u8 index){
	return bemf_read[index];
}


bool get_method(void){
	return sense_method;
}






