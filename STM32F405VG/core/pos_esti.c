#include "../core/pos_esti.h"

s32 position = 0;

s32 x[8] = {0};

s32 result[3] = {0};

u8 k = 0;

s32 ksin = 0;

s32 kcos = 0;

s32 bitch [1024] = {0};

s32 get_pos(void){
	return position;
}

void pos_update(void){
	
	DAC_enable(DAC_A);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	x[0] = (s32)(get_pk2pk())<<7;
	x[6] = x[0];	
		
	DAC_enable(DAC_A);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_B);
	while(!adc_done()){}
	x[1] = (s32)(get_pk2pk())<<7;
	x[7] = x[1];
		
	DAC_enable(DAC_B);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_C);
	while(!adc_done()){}
	x[3] =(s32)(get_pk2pk())<<7;
	
		
	DAC_enable(DAC_B);
	FET_gnd(FET_C);
	reset_dma_adc(SENSE_A);
	while(!adc_done()){}
	x[2] = (s32)(get_pk2pk())<<7;
	
		
	DAC_enable(DAC_C);
	FET_gnd(FET_A);
	reset_dma_adc(SENSE_B);
	while(!adc_done()){}
	x[4] = (s32)(get_pk2pk())<<7;
		
	DAC_enable(DAC_C);
	FET_gnd(FET_B);
	reset_dma_adc(SENSE_A);
	while(!adc_done()){}
	x[5] = (s32)(get_pk2pk())<<7;
	
	
	DAC_enable(DAC_DISABLE);
	FET_gnd(NO_FET);
	
	s16 offset = (x[0]+x[1]+x[2]+x[3]+x[4]+x[5])/6;
		
	x[0] = x[0]- offset;
	x[1] = x[1]- offset;
	x[2] = x[2]- offset;
	x[3] = x[3]- offset;
	x[4] = x[4]- offset;
	x[5] = x[5]- offset;
	x[6] = x[6]- offset;
	x[7] = x[7]- offset;
		
	k = 0;
		
	ksin = ((x[0+k]-x[3+k])+(x[1+k]+x[5+k])-(x[2+k]+x[4+k]))>>9;
	kcos = ((x[1+k]+x[2+k])-(x[4+k]+x[5+k]))/443;
		
	result[k] = app_atan2(ksin,kcos);
	
	
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
	
	
	
	position = median(3,result);
	
	position = (-position*73/36000)-6;
	if(position<0){position+=72;}
	
	u16 temp = get_abs();
	s16 temp1 = position-temp%73;
	if(temp1<-35){temp1+=72;}
	if(temp1>35){temp1-=72;}
	
	
	bitch[temp] = temp1;
	
/*	uart_tx(COM1,"x=[");		
	for(u16 i=0;i<6;i++){
		uart_tx(COM1,"%d,",x[i]);
		_delay_ms(10);
	}
	uart_tx(COM1,"];");
	uart_tx(COM1,"%d",position);*/
	
	
	//while(1);
}


void uart_bitch(void){
	uart_tx(COM1,"y=[");		
	for(u16 i=0;i<1024;i++){
		uart_tx(COM1,"%d,",bitch[i]);
		_delay_ms(10);
	}
	uart_tx(COM1,"];");
}