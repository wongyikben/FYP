#ifndef _POS_ESTI_H
#define _POS_ESTI_H

#include "stm32f4xx.h"
#include "stdbool.h"

// Debug Function 
void pos_update_induc(void);

void bemf_test(void);

void pos_update_bemf(void);

void uart_bitch(void);

void TIM5_init(void);

void TIM5_IRQHandler(void);

u32 get_k(void);

u16 get_fake_abs_enc(void);

u32 get_read(u8 index);

u32 get_induc(u8 index);

s16 get_bemf_read(u8 index);

// User Function

// get_pos return the current electrical degree of the rotor
s32 get_pos(void);

// position_update is called to update current position
void position_update(void);

// get_method will return the state of the current sensing method 1-High frequency injection 0-Back EMF
bool get_method(void);

// return the velocity
s32 get_vel(void);

// return the incremental encoder count
s32 get_enc(void);

// Init the sensing related IO and peripheral 
void sense_init(void);

#endif

