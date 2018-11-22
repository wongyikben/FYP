#ifndef _POS_ESTI_H
#define _POS_ESTI_H

#include "adc.h"
#include "../dac.h"
#include "../absEnc.h"
#include "trigon_math.h"
#include "uart.h"

s32 get_pos(void);

void pos_update(void);

void uart_bitch(void);





#endif

