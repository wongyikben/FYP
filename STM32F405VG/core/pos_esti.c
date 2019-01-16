#include "../core/pos_esti.h"
#include "macro_math.h"
#include "../core/vec.h"
#include "approx_math.h"

#define DELAY 7
#define BEMF_DELAY 1

s32 position = 0;
s32 last_position = 0;
s32 curr_position = 0;
s32 next_postion=0;

s32 last_pos = 0;
s32 enc = 0;
s32 last_enc = 0;
s32 enc_vel = 0;


#define VEL_BUFFER 20

s16 vel_buffer[VEL_BUFFER];
s16 mean_vel = 0;
u32 vel_count = 0;

u8 ratio = 80;

u8 true_count = 0;
s16 last_error = 0;
s32 x[8] = {0};



s32 lastx[6]={0};
s32 result[3] = {0};
u8 k = 0;
s32 ksin = 0;
s32 kcos = 0;
s32 offset = 0;
u32 max[6]={0};
u32 min[6]={0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};


vec3 x_vec[2];
vec3 read;

s32 save_x[6][72]={0};

s32 WTF[3][1024] = {0};



u16 x_offset[6]={0};

u16 init_pos = 0;

bool method_flag = false;

#define SENSE_VEL 5
bool sense_method = true;
u16 sense_count = 0;


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
	if(mean_vel>=0){
  	position+=(mean_vel)-5;
	}
	if(mean_vel<0){
		position+=mean_vel-3;
	}
	position%=73;
	if(position<0){position+=72;};
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


void induc_sense(void){

	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	_delay_us(DELAY);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	_delay_us(DELAY);
	reset_dma_adc(SENSE_A);
	read.n[0] = (s32)(get_pk2pk(SENSE_C))<<6;
	

	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	_delay_us(DELAY);	
	reset_dma_adc(SENSE_B);
	read.n[1] = (s32)(get_pk2pk(SENSE_A))<<6;
		

	
	while(!adc_done()){}
	read.n[2] = (s32)(get_pk2pk(SENSE_B))<<6;			
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);


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

void mat_init(void){
	vec3 temp;

	
	x_vec[0].n[0]=1;
	x_vec[0].n[1]=1;
	x_vec[0].n[2]=1;
	
	induc_sense();
	
	for (u8 i=0;i<10;i++){	
		N_method();
	}
	
	
}


s32 get_k(void){
	s32 k=0;
	s32 tri_x[3]={0};
	u16 temp = get_abs();
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	DAC_enable(DAC_A);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_B);	
	x[0] = (s32)(get_pk2pk(SENSE_C));
		
	if(true_count){
	x[0]=(x[0]*ratio+lastx[0]*(100-ratio))/100;	}
	lastx[0]=x[0];		
		
		
	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_C);	
	x[1] = (s32)(get_pk2pk(SENSE_B));
	if(true_count){
	x[1]=(x[1]*ratio+lastx[1]*(100-ratio))/100;	}
	lastx[1]=x[1];		
		

	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_A);
	x[3] =(s32)(get_pk2pk(SENSE_C));
	if(true_count){	
	x[3]=(x[3]*ratio+lastx[3]*(100-ratio))/100;	}
	lastx[3]=x[3];		
			
		

	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_B);
	x[2] = (s32)(get_pk2pk(SENSE_A));
	if(true_count){
	x[2]=(x[2]*ratio+lastx[2]*(100-ratio))/100;	}
	lastx[2]=x[2];	
	
	
	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_A);	
	x[4] = (s32)(get_pk2pk(SENSE_B));	
	if(true_count){
	x[4]=(x[4]*ratio+lastx[4]*(100-ratio))/100;	}
	lastx[4]=x[4];		

	while(!adc_done()){}
	x[5] = (s32)(get_pk2pk(SENSE_A));
	if(true_count){
	x[5]=(x[5]*ratio+lastx[5]*(100-ratio))/100;	}
	lastx[5]=x[5];	
		
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);

	
	x_vec[0].n[0]=1;
	x_vec[0].n[1]=1;
	x_vec[0].n[2]=1;
	
	//N_method(0);

	
  x_vec[1].n[0]=1;
	x_vec[1].n[1]=1;
	x_vec[1].n[2]=1;
	
	//N_method(1);

	// Position estimation code 
	
	tri_x[0] = (x_vec[0].n[0]+x_vec[1].n[0])<<7;
	tri_x[1] = (x_vec[0].n[1]+x_vec[1].n[1])<<7;
	tri_x[2] = (x_vec[0].n[2]+x_vec[1].n[2])<<7;
	
	tri_x[0] = x_vec[0].n[0];
	tri_x[1] = x_vec[0].n[1];
	tri_x[2] = x_vec[0].n[2];


	offset = (tri_x[0]+tri_x[1]+tri_x[2])/3;

	tri_x[0]-=offset;
	tri_x[1]-=offset;
	tri_x[2]-=offset;


	ksin = (tri_x[0]-tri_x[1]-tri_x[2])>>8;
	kcos = (tri_x[1]-tri_x[2])/222;
	
	k = Sqrt(ksin*ksin+kcos*kcos);
	return k;

}




void pos_update_bemf(void){

	u16 temp = get_abs();
	// 1 0 2
	FET_gnd(FET_B);
	_delay_us(BEMF_DELAY);
	reset_dma_adc_bemf(SENSE_C);
	while(!adc_done()){}
	FET_gnd(FET_C);
	_delay_us(BEMF_DELAY);
	reset_dma_adc_bemf(SENSE_A);
	x[1] = (s32)(get_bemf(SENSE_C));
	WTF[1][temp]=x[1];
	x[1]<<=7;

	while(!adc_done()){}
	FET_gnd(FET_A);
	_delay_us(BEMF_DELAY);	
	reset_dma_adc_bemf(SENSE_B);
	x[2] = (s32)(get_bemf(SENSE_A));
	WTF[2][temp]=x[2];
	x[2]<<=7;
		
		

	while(!adc_done()){}
	x[0] = (s32)(get_bemf(SENSE_B));
	WTF[0][temp]=x[0];	
  x[0]<<=7;		
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);
		
		
	

	ksin = (x[0]-x[1]-x[2])>>9;
	kcos = (x[1]-x[2])/443;

		
		
	position = (-app_atan2(ksin,kcos)-9000);
	if(mean_vel>0){
		position+=9000;
	}else{
		position-=9000;
	}	
	
	position/=250;
	
	if(position<0){
		position+=144;
	}
	position%=144;

		
	if(ABS(position*true_count-last_pos)>72){
		if(position*true_count-last_pos<0){
			enc+= position*true_count-last_pos+144;
		}else{
			enc+= position*true_count-last_pos-144;
		}
	
	}else{
			enc += position*true_count-last_pos;
	}
	last_pos = position;
	
	
	// velocity 
	enc_vel = enc-last_enc;
	last_enc=enc;

	
	//position+=enc_vel;

	
	// error calculation 
	position = (position*146/144);
	
	vel_buffer[vel_count++%VEL_BUFFER] = enc_vel;
	mean_vel = 0;
	for (u8 i=0;i<VEL_BUFFER;i++){
		mean_vel +=vel_buffer[i];
	}
	mean_vel/=VEL_BUFFER;

	true_count=1;

}


void pos_update4(void){
		
	s32 tri_x[3]={0};
	
	induc_sense();

	u16 temp = get_abs();
		
	
	N_method();
	N_method();

/*	vec3 offse;
	
	
	v_funct(x_vec[0]);
	offse.n[0]=output[0];
	offse.n[1]=output[1];
	offse.n[2]=output[2];
	uart_tx(COM1,"%d\n",vec_length(vec_sub(offse,read))>>10);*/

	// Position estimation code 
	
	
	tri_x[0] = x_vec[0].n[0]<<7;
	tri_x[1] = x_vec[0].n[1]<<7;
	tri_x[2] = x_vec[0].n[2]<<7;


	offset = (tri_x[0]+tri_x[1]+tri_x[2])/3;

	tri_x[0]-=offset;
	tri_x[1]-=offset;
	tri_x[2]-=offset;


	ksin = (tri_x[0]-tri_x[1]-tri_x[2])>>9;
	kcos = (tri_x[1]-tri_x[2])/443;

	position = (app_atan2(ksin,kcos)+9000)/500;
	position%=72;
	
	// mean filter 
/*		if(ABS(position-last_position)>36){
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
	position = curr_position;*/
	if(position<0){position+=71;}
	position%=72;
	
	

	
	// translate to encoder 
	
	if(ABS(position*true_count-last_pos)>36){
		if(position*true_count-last_pos<0){
			enc+= position*true_count-last_pos+72;
		}else{
			enc+= position*true_count-last_pos-72;
		}
	
	}else{
			enc += position*true_count-last_pos;
	}
	last_pos = position;
	
	
	// velocity 
	enc_vel = enc-last_enc;
	last_enc=enc;

	

	// error calculation 
	position = (position*73/72);

	
	vel_buffer[vel_count++%VEL_BUFFER] = enc_vel;
	mean_vel = 0;
	for (u8 i=0;i<VEL_BUFFER;i++){
		mean_vel +=vel_buffer[i];
	}
	mean_vel/=VEL_BUFFER;

	
	
	
	
	true_count=1;
}




void pos_update2(void){
	
	
	u16 temp = get_abs();
	
	if(!method_flag){
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	DAC_enable(DAC_A);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_B);	
	x[0] = (s32)(get_pk2pk(SENSE_C))<<7;
		
	//max[0]=x[0];
	if(true_count){
	x[0]=(x[0]*ratio+lastx[0]*(100-ratio))/100;	}
	lastx[0]=x[0];	
	x[6] = x[0];	
		
		
	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_C);	
	x[1] = (s32)(get_pk2pk(SENSE_B))<<7;
	//max[1]=x[1];
	if(true_count){
	x[1]=(x[1]*ratio+lastx[1]*(100-ratio))/100;	}
	lastx[1]=x[1];	
	x[7] = x[1];	
		

	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_A);
	x[3] =(s32)(get_pk2pk(SENSE_C))<<7;
	//max[3]=x[3];
	if(true_count){	
	x[3]=(x[3]*ratio+lastx[3]*(100-ratio))/100;	}
	lastx[3]=x[3];		
			
		

	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_B);
	x[2] = (s32)(get_pk2pk(SENSE_A))<<7;
	//max[2]=x[2];	
	if(true_count){
	x[2]=(x[2]*ratio+lastx[2]*(100-ratio))/100;	}
	lastx[2]=x[2];	
	
	
	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_A);	
	x[4] = (s32)(get_pk2pk(SENSE_B))<<7;
	//max[4]=x[4];	
	if(true_count){
	x[4]=(x[4]*ratio+lastx[4]*(100-ratio))/100;	}
	lastx[4]=x[4];		

	while(!adc_done()){}
	x[5] = (s32)(get_pk2pk(SENSE_A))<<7;
	//max[5]=x[5];
	if(true_count){
	x[5]=(x[5]*ratio+lastx[5]*(100-ratio))/100;	}
	lastx[5]=x[5];	
		
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);
	
		
	
		
/*	if(!method_flag){
		if(x[0]>max[0]){max[0]=x[0];}
		if(x[0]<min[0]){min[0]=x[0];}	
		if(x[1]>max[1]){max[1]=x[1];}
		if(x[1]<min[1]){min[1]=x[1];}	
		if(x[3]>max[3]){max[3]=x[3];}
		if(x[3]<min[3]){min[3]=x[3];}		
		if(x[2]>max[2]){max[2]=x[2];}
		if(x[2]<min[2]){min[2]=x[2];}
		if(x[4]>max[4]){max[4]=x[4];}
		if(x[4]<min[4]){min[4]=x[4];}		
		if(x[5]>max[5]){max[5]=x[5];}
		if(x[5]<min[5]){min[5]=x[5];}	
		
		
	
		x_offset[0] = offset;
		x_offset[1] = offset;
		x_offset[2] = offset;
		x_offset[3] = offset;
		x_offset[4] = offset;
		x_offset[5] = offset;
		
		s16 eerr = temp - init_pos;
		if(eerr>512){eerr-=1024;}
		if(eerr<-512){eerr+=1024;}
		
		if(ABS(eerr)>73*2){
			method_flag = true;
		
			x_offset[0] = (max[0]+offset+min[0])>>2;
		  x_offset[1] = (max[1]+offset+min[1])>>2;
		  x_offset[2] = (max[2]+offset+min[2])>>2;
			x_offset[3] = (max[3]+offset+min[3])>>2;
			x_offset[4] = (max[4]+offset+min[4])>>2;
			x_offset[5] = (max[5]+offset+min[5])>>2;
		}
	
	
	}*/
	

	// Position estimation code 
		
	offset = (x[0]+x[1]+x[2]+x[3]+x[4]+x[5])/6;
	
	x[0]-=offset;
	x[1]-=offset;
	x[2]-=offset;
	x[3]-=offset;
	x[4]-=offset;
	x[5]-=offset;
	x[6]-=offset;
	x[7]-=offset;
		
	k = 0;
		
	ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
		
	result[k] = app_atan2(ksin,kcos);
	
	bitch1[0][temp]=ksin;
	bitch1[1][temp]=kcos;
	
	k++;
	
  ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
	
	result[k] = app_atan2(ksin,kcos);
	result[k]-= 6000;
	if(result[k]<-18000){
		result[k]+=36000;
	}
	
	
	k++;
	
  ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
		
	result[k] = app_atan2(ksin,kcos);
	result[k]-= 12000;
	if(result[k]<-18000){
		result[k]+=36000;
	}
	
	
	position = median(3,result)/500;
	if(position<0){
		position +=72;
	}
	
	// mean filter 
	
	
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
	//last_position = curr_position;
	
	
	// translate to encoder 
	
	if(ABS(position*true_count-last_pos)>36){
		if(position*true_count-last_pos<0){
			enc+= position*true_count-last_pos+72;
		}else{
			enc+= position*true_count-last_pos-72;
		}
	
	}else{
			enc += position*true_count-last_pos;
	}
	last_pos = position;
	
	
	// velocity 
	enc_vel = enc-last_enc;
	last_enc=enc;
	if(ABS(enc_vel)<2){
		ratio = 80;
	}
	if(ABS(enc_vel)>=2){
		ratio=95;
	}
	
 if(ABS(enc_vel)>=4){
		ratio=100;
	}
	
	
	for(u8 i=0;i<6;i++){
		if(save_x[i][position]==0){
			save_x[i][position] = (x[i]+offset);
		}else{
			save_x[i][position] = (((x[i]+offset))+ save_x[i][position]*9)/10;
		}
	}
	
	true_count=1;
	//check 
	for(u8 i=0;i<72;i++){
		if(save_x[0][i]==0){
			break;
		}
		if(i==71){
			method_flag = true;
			true_count=0;
			lastx[0]=0;
			lastx[1]=0;
			lastx[2]=0;
			lastx[3]=0;
			lastx[4]=0;
			lastx[5]=0;
			uart_tx(COM1,"y=[");
			for(u8 i=0;i<6;i++){
				for(u8 j=0;j<72;j++){
					uart_tx(COM1,"%d,",save_x[i][j]);
					_delay_ms(5);
				}
				uart_tx(COM1,";");
			}uart_tx(COM1,"];");
			
			while(1){}
			
		}
	}
	

	
}else{
	
	
	
	
	
	
	
	for(u8 j=0;j<6;j++){
		x_offset[j] = 0;
		max[j]=0;
		min[j]= 0xffff;
		for(u8 i=0;i<72;i++){
			if(save_x[j][i]>max[j]){max[j]=save_x[j][i];}
			if(save_x[j][i]<min[j]){min[j]=save_x[j][i];}
			x_offset[j]+=save_x[j][i];
		}
		x_offset[j]/=72;
	}
	
	
	
	
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	DAC_enable(DAC_A);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_B);	
	x[0] = (s32)(get_pk2pk(SENSE_C))<<7;
	x[0] -=x_offset[0];
	x[0]/=(max[0]-min[0])/10;		
		

	if(true_count){
	x[0]=(x[0]*ratio+lastx[0]*(100-ratio))/100;	}
	lastx[0]=x[0];	
	x[6] = x[0];	
		
		
	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_C);	
	x[1] = (s32)(get_pk2pk(SENSE_B))<<7;
	x[1] -=x_offset[1];
	x[1]/=(max[1]-min[1])/10;		
		
		
	
	if(true_count){
	x[1]=(x[1]*ratio+lastx[1]*(100-ratio))/100;	}
	lastx[1]=x[1];	
	x[7] = x[1];	
		

	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_A);
	x[3] =(s32)(get_pk2pk(SENSE_C))<<7;
	x[3] -=x_offset[3];
	x[3]/=(max[3]-min[3])/10;		
		
		
		
		
		
	if(true_count){	
	x[3]=(x[3]*ratio+lastx[3]*(100-ratio))/100;	}
	lastx[3]=x[3];		
			
		

	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_B);
	x[2] = (s32)(get_pk2pk(SENSE_A))<<7;
	x[2] -=x_offset[2];
	x[2]/=(max[2]-min[2])/10;		
		
		
	if(true_count){
	x[2]=(x[2]*ratio+lastx[2]*(100-ratio))/100;	}
	lastx[2]=x[2];	
	
	
	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_A);	
	x[4] = (s32)(get_pk2pk(SENSE_B))<<7;
	x[4] -=x_offset[4];
	x[4]/=(max[4]-min[4])/10;	
		
		
		
	if(true_count){
	x[4]=(x[4]*ratio+lastx[4]*(100-ratio))/100;	}
	lastx[4]=x[4];		

	while(!adc_done()){}
	x[5] = (s32)(get_pk2pk(SENSE_A))<<7;
	x[5] -=x_offset[5];
	x[5]/=(max[5]-min[5])/10;	
		
	if(true_count){
	x[5]=(x[5]*ratio+lastx[5]*(100-ratio))/100;	}
	lastx[5]=x[5];	
		
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);
	
	

	k = 0;
		
	ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>6;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/55;
		
	result[k] = app_atan2(ksin,kcos);
	
	//bitch1[0][temp]=ksin;
	//bitch1[1][temp]=kcos;
	
	k++;
	
  ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>6;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/55;
	
	result[k] = app_atan2(ksin,kcos);
	result[k]-= 6000;
	if(result[k]<-18000){
		result[k]+=36000;
	}
	
	
	k++;
	
  ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>6;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/55;
		
	result[k] = app_atan2(ksin,kcos);
	result[k]-= 12000;
	if(result[k]<-18000){
		result[k]+=36000;
	}
	
	
	position = median(3,result)/500;
	if(position<0){
		position +=72;
	}
	
	// mean filter 
	
	
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
	//last_position = curr_position;
	
	
	// translate to encoder 
	
	if(ABS(position*true_count-last_pos)>36){
		if(position*true_count-last_pos<0){
			enc+= position*true_count-last_pos+72;
		}else{
			enc+= position*true_count-last_pos-72;
		}
	
	}else{
			enc += position*true_count-last_pos;
	}
	last_pos = position;
	
	
	// velocity 
	enc_vel = enc-last_enc;
	last_enc=enc;
	if(ABS(enc_vel)<2){
		ratio = 80;
	}
	if(ABS(enc_vel)>=2){
		ratio=95;
	}
	
 if(ABS(enc_vel)>=4){
		ratio=100;
	}

	
	
	for(u8 i=0;i<6;i++){
			save_x[i][position] = ((x[i]*10/(max[i]-min[i]))+x_offset[i]+ save_x[i][position]*9)/10;
	}
	

	true_count=1;

}

	position = (position*73/72)+36;
	if(position<0){position+=72;}
			
		
}


void pos_update(void){
	u16 temp = get_abs();
	
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	DAC_enable(DAC_A);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_B);	
	x[0] = (s32)(get_pk2pk(SENSE_C))<<7;
		
	max[0]=x[0];
	if(true_count){
	x[0]=(x[0]*ratio+lastx[0]*(100-ratio))/100;	}
	lastx[0]=x[0];	
	x[6] = x[0];	
		
		
	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_C);	
	x[1] = (s32)(get_pk2pk(SENSE_B))<<7;
	max[1]=x[1];
	if(true_count){
	x[1]=(x[1]*ratio+lastx[1]*(100-ratio))/100;	}
	lastx[1]=x[1];	
	x[7] = x[1];	
		

	while(!adc_done()){}
	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_A);
	x[3] =(s32)(get_pk2pk(SENSE_C))<<7;
	max[3]=x[3];
	if(true_count){	
	x[3]=(x[3]*ratio+lastx[3]*(100-ratio))/100;	}
	lastx[3]=x[3];		
			
		

	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_B);
	x[2] = (s32)(get_pk2pk(SENSE_A))<<7;
	max[2]=x[2];	
	if(true_count){
	x[2]=(x[2]*ratio+lastx[2]*(100-ratio))/100;	}
	lastx[2]=x[2];	
	
	
	while(!adc_done()){}
	DAC_enable(DAC_C);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_A);	
	x[4] = (s32)(get_pk2pk(SENSE_B))<<7;
	max[4]=x[4];	
	if(true_count){
	x[4]=(x[4]*ratio+lastx[4]*(100-ratio))/100;	}
	lastx[4]=x[4];		

	while(!adc_done()){}
	x[5] = (s32)(get_pk2pk(SENSE_A))<<7;
	max[5]=x[5];
	if(true_count){
	x[5]=(x[5]*ratio+lastx[5]*(100-ratio))/100;	}
	lastx[5]=x[5];	
		
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);
	
		
	
		
/*	if(!method_flag){
		if(x[0]>max[0]){max[0]=x[0];}
		if(x[0]<min[0]){min[0]=x[0];}	
		if(x[1]>max[1]){max[1]=x[1];}
		if(x[1]<min[1]){min[1]=x[1];}	
		if(x[3]>max[3]){max[3]=x[3];}
		if(x[3]<min[3]){min[3]=x[3];}		
		if(x[2]>max[2]){max[2]=x[2];}
		if(x[2]<min[2]){min[2]=x[2];}
		if(x[4]>max[4]){max[4]=x[4];}
		if(x[4]<min[4]){min[4]=x[4];}		
		if(x[5]>max[5]){max[5]=x[5];}
		if(x[5]<min[5]){min[5]=x[5];}	
		
		
	
		x_offset[0] = offset;
		x_offset[1] = offset;
		x_offset[2] = offset;
		x_offset[3] = offset;
		x_offset[4] = offset;
		x_offset[5] = offset;
		
		s16 eerr = temp - init_pos;
		if(eerr>512){eerr-=1024;}
		if(eerr<-512){eerr+=1024;}
		
		if(ABS(eerr)>73*2){
			method_flag = true;
		
			x_offset[0] = (max[0]+offset+min[0])>>2;
		  x_offset[1] = (max[1]+offset+min[1])>>2;
		  x_offset[2] = (max[2]+offset+min[2])>>2;
			x_offset[3] = (max[3]+offset+min[3])>>2;
			x_offset[4] = (max[4]+offset+min[4])>>2;
			x_offset[5] = (max[5]+offset+min[5])>>2;
		}
	
	
	}*/
	

	// Position estimation code 
		
	offset = (x[0]+x[1]+x[2]+x[3]+x[4]+x[5])/6;
	
	x[0]-=offset;
	x[1]-=offset;
	x[2]-=offset;
	x[3]-=offset;
	x[4]-=offset;
	x[5]-=offset;
	x[6]-=offset;
	x[7]-=offset;
		
	k = 0;
		
	ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
		
	result[k] = app_atan2(ksin,kcos);
	
	bitch1[0][temp]=ksin;
	bitch1[1][temp]=kcos;
	
	k++;
	
  ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
	
	result[k] = app_atan2(ksin,kcos);
	result[k]-= 6000;
	if(result[k]<-18000){
		result[k]+=36000;
	}
	
	
	k++;
	
  ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
		
	result[k] = app_atan2(ksin,kcos);
	result[k]-= 12000;
	if(result[k]<-18000){
		result[k]+=36000;
	}
	
	
	position = median(3,result)/500;
	if(position<0){
		position +=72;
	}
	
	// mean filter 
	
	
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
	//last_position = curr_position;
	
	
	// translate to encoder 
	
	if(ABS(position*true_count-last_pos)>36){
		if(position*true_count-last_pos<0){
			enc+= position*true_count-last_pos+72;
		}else{
			enc+= position*true_count-last_pos-72;
		}
	
	}else{
			enc += position*true_count-last_pos;
	}
	last_pos = position;
	
	
	// velocity 
	enc_vel = enc-last_enc;
	last_enc=enc;
	if(ABS(enc_vel)<2){
		ratio = 80;
	}	
	if(ABS(enc_vel)>=2){
		ratio=95;
	}
	
 if(ABS(enc_vel)>=4){
		ratio=100;
	}
	
	

	
	
	
	
	
	// error calculation 
	position = (position*73/72)+36;
	if(position<0){position+=72;}
	

	s16 temp1 = position-temp%73;
	if(temp1<-35){temp1+=72;}
	if(temp1>35){temp1-=72;}
	
	
/*	if(temp == 140){
		bitch[bitch_index++] = temp1;
		if(bitch_index>1023){
			bitch_index=1023;
		}
	}*/
	
	bitch[temp] = temp1;//(temp1*4+last_error*6)/10;
	//last_error = temp1;
	true_count=1;
/*	if(ABS(position-50)>20){
		uart_tx(COM1,"%d\n",max[0]);
		uart_tx(COM1,"%d\n",max[1]);
		uart_tx(COM1,"%d\n",max[2]);
		uart_tx(COM1,"%d\n",max[3]);
		uart_tx(COM1,"%d\n",max[4]);
		uart_tx(COM1,"%d\n\n",max[5]);
		uart_tx(COM1,"%d %d %d\n",result[0],result[1],result[2]);
		while(1){}
	
	}*/
	
}


void uart_bitch(void){
	uart_tx(COM1,"y=[");		
	for(u16 i=0;i<1024;i++){
		uart_tx(COM1,"%d,",WTF[0][i]);
		uart_tx(COM1,"%d,",WTF[1][i]);
		uart_tx(COM1,"%d;",WTF[2][i]);
//	uart_tx(COM1,"%d,",bitch1[1][i]);
//	uart_tx(COM1,"%d;",bitch1[2][i]);
		_delay_ms(10);
	}
	
/*	x_offset[0] = (max[0]+min[0])>>1;
	x_offset[1] = (max[1]+min[1])>>1;
	x_offset[2] = (max[2]+min[2])>>1;
	x_offset[3] = (max[3]+min[3])>>1;
	x_offset[4] = (max[4]+min[4])>>1;
	x_offset[5] = (max[5]+min[5])>>1;*/
	
	//uart_tx(COM1,"%d,\n",bitch_index);
/*	uart_tx(COM1,"%d,",x_offset[0]>>7);
	uart_tx(COM1,"%d,",x_offset[1]>>7);
	uart_tx(COM1,"%d,",x_offset[2]>>7);
	uart_tx(COM1,"%d,",x_offset[3]>>7);
	uart_tx(COM1,"%d,",x_offset[4]>>7);
	uart_tx(COM1,"%d,",x_offset[5]>>7);*/
	
	uart_tx(COM1,"];\n");
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
	uart_tx(COM1,"%d,",error);
	last = curr;
	next = (curr+vel)%1024;*/

	
}

s32 get_vel(void){
	return mean_vel;
}


s32 get_enc(void){
	return enc;
}





void position_update(void){
	
	if(sense_method){
		// high frequency injection
	
		pos_update4();
	
	
	}else{
		// BEMF
		pos_update_bemf();
	
	}

	if(sense_count++>100){
		if(ABS(mean_vel)<SENSE_VEL&&!sense_method){
				sense_count = 0;
				sense_method = true;
		}
		if(ABS(mean_vel)>=SENSE_VEL&&sense_method){
				sense_count = 0;
				sense_method = false;
		}
	}
}

bool get_method(void){
	return sense_method;
}






