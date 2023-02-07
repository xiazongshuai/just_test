#ifndef _DRV_INIT_H
#define _DRV_INIT_H

#include "stm32f10x.h"
#include "systick.h"
#include "ir_io_check.h"
#include "motor.h"
#include "soft_iic.h"
#include "timer.h"
#include "hc_sr04.h"
#include "usart.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "handle_console_pid.h"


void drv_init(void);
void app_uart2_init(void);
	
#endif