#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

typedef enum led_dev
{
  led_on = 0,
  led_off,
  led_blink,
  led_breath,
}LED_DEV;

void led_init(void);
void set_led_sta(LED_DEV led_sta);
void led_tick(void);
#endif