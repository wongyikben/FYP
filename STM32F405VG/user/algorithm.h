/*
 * @file		algorithm.h
 * @brief 	calculation of position based on three encoder system
 * @author	Ryusuke SUGIMOTO (rsugimoto@ust.hk)
 * @date		January 6, 2018
 */

#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#include <math.h>
#include "stm32f4xx.h"
#include "define.h"
#include "encoder.h"
#include "approx_math.h"
#include "gun_can_motor.h"

#define M_PI 3.14159265358979323846264338327950288

extern s16 real_X;
extern s16 real_Y;
extern s16 real_A;

void calculation(void);
void calculation_init(void);

#endif		/* __ALGORITHM_H */
