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

void pos_update3(void);

void pos_update4(void);

void pos_update_bemf(void);

void uart_bitch(void);




void TIM5_init(void);

void TIM5_IRQHandler(void);

void mat_init(void);




s32 get_vel(void);

s32 get_enc(void);

s32 get_k(void);
// Final version
void position_update(void);

bool get_method(void);
#endif

