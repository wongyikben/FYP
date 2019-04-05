#include "absEnc.h"


void absEnc_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u16 gray2bin(u16 gray){
	u16 result = 0;
		result |= (gray ^ (result >> 1)) & 0x0200;
	  result |= (gray ^ (result >> 1)) & 0x0100;
    result |= (gray ^ (result >> 1)) & 0x0080;
    result |= (gray ^ (result >> 1)) & 0x0040;
    result |= (gray ^ (result >> 1)) & 0x0020;
    result |= (gray ^ (result >> 1)) & 0x0010;
    result |= (gray ^ (result >> 1)) & 0x0008;
    result |= (gray ^ (result >> 1)) & 0x0004;
    result |= (gray ^ (result >> 1)) & 0x0002;
    result |= (gray ^ (result >> 1)) & 0x0001;
    return result;
}


u16 get_abs(){
	return gray2bin(~(GPIOB->IDR)&0x03FF);
	
}

u16 get_abs_angle(){
return ((73-((get_abs()+31)%73))%73); //Ben 31 Andy 66
}
