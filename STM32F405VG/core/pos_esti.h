#ifndef _POS_ESTI_H
#define _POS_ESTI_H

#include "adc.h"
#include "../dac.h"
#include "../absEnc.h"
#include "trigon_math.h"
#include "uart.h"

s32 get_pos(void);

s32 get_pos2(void);

void pos_update(void);

void pos_update2(void);

void uart_bitch(void);

void TIM5_init(void);

void Rcal(s32* upper,s32* lower);

void TIM5_IRQHandler(void);

s16 get_vel(void);





#endif

