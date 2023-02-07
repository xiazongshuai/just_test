#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

#define KEY_SHORT   1
#define KEY_DOUBLE  2
#define KEY_LONG    3

void key_init(void);
int get_key_sta(void);
void check_key_sta(void);

#endif