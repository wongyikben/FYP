#ifndef _ABSENC_H
#define _ABSENC_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

void absEnc_init(void);

u16 get_abs(void);

u16 gray2bin(u16 gray);

u16 get_abs_angle(void);

#endif
