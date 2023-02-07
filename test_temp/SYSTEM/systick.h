#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "stm32f10x.h"

void systick_init();
void systick_ms(u32 num);
void systick_s(u32 num);
u32 millis(void);
#endif
