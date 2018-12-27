#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx_adc.h"
#include "gpio.h"
#include "../absEnc.h"
#include "uart.h"
#include "ticks.h"


enum Sense_Phase{
	SENSE_A=0,SENSE_B,SENSE_C
};


void DMA2_Stream0_IRQHandler(void);

void adc_gpio_init(void);

void adc_dma_init(u8 input);

void adc_init(void);

void reset_dma_adc(u8 input);

void pk2pk(u8 input);

void uart_reading(void);

u16 get_pk2pk(u8 input);

u32 get_count(void);

bool adc_done(void);

s32 median(u8 n, s32* x);






#endif
