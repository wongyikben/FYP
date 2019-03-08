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

#define DELAY 7
#define BEMF_DELAY 1
#define K_THRESH 6000

s32 position = 0;
s32 last_deg = 0;
s32 last_position = 0;
s32 curr_position = 0;

s32 last_pos = 0;
s32 enc = 0;
s32 last_enc = 0;
s32 enc_vel = 0;




u8 true_count = 0;
s32 x[3] = {0};



s32 result[3] = {0};
u32 k = 0;
s32 ksin = 0;
s32 kcos = 0;
s32 offset = 0;



vec3 x_vec[2];
vec3 read;
vec3 last_read;


s32 WTF[3][73] = {0};



u16 x_offset[6]={0};

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
	last_enc=enc;

}

void induc_sense(void){

	u16 temp = get_abs();
	
	//DAC_enable(Mode|DAC_A);
	GPIOC->ODR = ((GPIOC->ODR)&(0xC307))|(0x0008);
	FET_gnd(FET_B);
	_delay_us(DELAY);
  reset_dma_adc(SENSE_C);
	
	
	while(!adc_done()){}

	//DAC_enable(Mode|DAC_B);
	GPIOC->ODR = ((GPIOC->ODR)&(0xC307))|(0x0010);
	FET_gnd(FET_C);
	_delay_us(DELAY);	
	reset_dma_adc(SENSE_A);
	read.n[0] = (s32)(get_pk2pk(SENSE_C))<<6;
	

	while(!adc_done()){}
	
	//DAC_enable(Mode|DAC_C);
	GPIOC->ODR = ((GPIOC->ODR)&(0xC307))|(0x0020);
	FET_gnd(FET_A);
	_delay_us(DELAY);	
	reset_dma_adc(SENSE_B);
	read.n[1] = (s32)(get_pk2pk(SENSE_A))<<6;
		

	
	while(!adc_done()){}
	read.n[2] = (s32)(get_pk2pk(SENSE_B))<<6;			
	DAC_enable(ALL_DISABLE);
	FET_gnd(NO_FET);
	
	//current_sensing_init();
	//uart_tx(COM3,"%d\n",read.n[2]>>6);
	// cheat
			
	/*if(last_read.n[0]!=0){
		if(ABS(last_read.n[0]-read.n[0])>3200){read.n[0]=last_read.n[0];}
		if(ABS(last_read.n[1]-read.n[1])>3200){read.n[1]=last_read.n[1];}
		if(ABS(last_read.n[2]-read.n[2])>3200){read.n[2]=last_read.n[2];}
	}	
		
	last_read = read;*/
				
	
	
		//current_senseing_re();

//	WTF[0][(temp+93)%73]=read.n[0]>>6;
//	WTF[1][(temp+93)%73]=read.n[1]>>6;
//	WTF[2][(temp+93)%73]=read.n[2]>>6;

}



void bemf_sense(void){
	DAC_enable(ALL_DISABLE);
	FET_gnd(FET_B);
	_delay_us(BEMF_DELAY);
	reset_dma_adc_bemf(SENSE_C);
	while(!adc_done()){}
	FET_gnd(FET_C);
	_delay_us(BEMF_DELAY);
	reset_dma_adc_bemf(SENSE_A);
	x[1] = (s32)(get_bemf(SENSE_C));
	//WTF[1][temp]=x[1];
	x[1]<<=7;

	while(!adc_done()){}
	FET_gnd(FET_A);
	_delay_us(BEMF_DELAY);	
	reset_dma_adc_bemf(SENSE_B);
	x[2] = (s32)(get_bemf(SENSE_A));
	//WTF[2][temp]=x[2];
	x[2]<<=7;
		
		

	while(!adc_done()){}
	x[0] = (s32)(get_bemf(SENSE_B));
	//WTF[0][temp]=x[0];	
  x[0]<<=7;		
	DAC_enable(ALL_DISABLE);
	FET_gnd(NO_FET);

	//current_sensing_init();
	//uart_tx(COM3,"%d %d %d\n",x[0]>>7,x[1]>>7,x[2]>>7);	
		
		
		
}

#define delta 	1



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
	//u8 vlevel = 5;
	//dac_init(150,vlevel);
	dac_init(220,5);
	DAC_enable_init();
	adc_init();
	vec3 temp;

	
	FET_gnd(FET_A);
	_delay_ms(10);
	ADC_midpt_cal(SENSE_A);
	
	FET_gnd(FET_B);
	_delay_ms(10);
	ADC_midpt_cal(SENSE_B);
	
	FET_gnd(FET_C);
	_delay_ms(10);
	ADC_midpt_cal(SENSE_C);
	
	last_deg = (get_abs()+93)%146;
	
	
	x_vec[0].n[0]=1;
	x_vec[0].n[1]=1;
	x_vec[0].n[2]=1;
	
	induc_sense();
	/*while((read.n[0]>>6)<490){
		vlevel++;
		dac_init(150,vlevel);
		adc_init();
		_delay_ms(500);
		induc_sense();
		if(vlevel==6){
			break;
		}
	}*/
	//uart_tx(COM3,"%d,%d\n",vlevel,read.n[0]>>6);
	
	

	for (u8 i=0;i<10;i++){	
		N_method();
	}
	
	pos_update_induc();
	
	
	
}





void pos_update_bemf(void){
	u16 temp = get_abs();

	bemf_sense();		
	
	

	ksin = (x[0]-x[1]-x[2])>>9;
	kcos = (x[1]-x[2])/443;

		
		
	position = (-app_atan2(ksin,kcos)-9000);
	
	//u32 tp = ksin*ksin+kcos*kcos;
	//if(tp>1300){
	//uart_tx(COM3,"%d,%d\n",ksin*ksin+kcos*kcos,mean_vel);}
	//if(positive){position+=9000;}else{position-=9000;}	
	
	position+=9000;
	position/=250;
	
	if(position<0){position+=144;}
	position%=144;
			s16 error_under=ABS(last_deg-position);
		s16 error_upper=ABS(last_deg-(72+position));
		if(error_under>72){error_under = 144-error_under;}
		if(error_upper>72){error_upper = 144-error_upper;}
		if(error_upper<error_under){position+=72;}	

	enc_cal();
	

	last_deg = position;
	// error calculation 
	//position = (position*146/144);
	

	true_count=1;
	

}


void pos_update_induc(void){
		
	//bemf_sense();
		
	s32 tri_x[3]={0};
	
	induc_sense();

	u16 temp = get_abs();
		
	
	N_method();
	N_method();
	N_method();
	

	
	// Position estimation code 
	//	WTF[0][(temp+93)%73]=x_vec[0].n[0];
  // 	WTF[1][(temp+93)%73]=x_vec[0].n[1];
  //	WTF[2][(temp+93)%73]=x_vec[0].n[2];
	
	// 2686 632
	// 2659 642
	// 2746 641
	
	//tri_x[0] = ((x_vec[0].n[0]-2686)*8192)/632;
	//tri_x[1] = ((x_vec[0].n[1]-2659)*8192)/642;
	//tri_x[2] = ((x_vec[0].n[2]-2746)*8192)/641;

	//uart_tx(COM3,"%d\n",(tri_x[0]+tri_x[1]+tri_x[2]));
	
	tri_x[0] = x_vec[0].n[0]<<7;
	tri_x[1] = x_vec[0].n[1]<<7;
	tri_x[2] = x_vec[0].n[2]<<7;
	
	offset = (tri_x[0]+tri_x[1]+tri_x[2])/3;

	tri_x[0]-=offset;
	tri_x[1]-=offset;
	tri_x[2]-=offset;
	
	
	

	ksin = (tri_x[0]-tri_x[1]-tri_x[2])>>9; //2^9 = 2*256
	kcos = (tri_x[1]-tri_x[2])/443;   //443 = 2sin(pi/3)*256
	position = app_atan2(ksin,kcos);

	//k = ksin*ksin + kcos*kcos;
	

	
	
	
	//uart_tx(COM3,"%d,",Sqrt(k)>>10);
	position = (position+9000)/500;
	position%=72;
	last_position%=72;
	
	// mean filter 
if(true_count){	
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



	position-=0;//????  I KNOW WHY LA Because this method did not seperate Ld Lq, so this offset is the cos(Ld/Lq) 
	if(position<0){position+=71;}
	position%=72;
	
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
	
		
}




void position_update(void){
	u16 temp = get_abs();
	
	

	bemf_sense();		
	offset = (x[0]+x[1]+x[2])/3;

	x[0]-=offset;
	x[1]-=offset;
	x[2]-=offset;
	

	ksin = (x[0]-x[1]-x[2])>>9;
	kcos = (x[1]-x[2])/443;
	
	k = ((ksin*ksin+kcos*kcos)>>1)+(k>>1);
	
//	uart_tx(COM3,"%d\n",k);
	
	if(k>K_THRESH){
		position = (-app_atan2(ksin,kcos)-9000);
			{position+=9000;}
			position/=250;
	
			if(position<0){position+=144;}
			position%=144;
			
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
			true_count=1;
			sense_method=false;
			return;
	}
	if(sense_method){pos_update_induc();}
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


bool get_method(void){
	return sense_method;
}






