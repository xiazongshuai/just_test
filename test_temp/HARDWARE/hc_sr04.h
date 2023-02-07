#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "stm32f10x.h"

extern int hc_time;

#define hc_tring_port  GPIOB
#define hc_tring_pin   GPIO_Pin_10
#define hc_tring_clk   RCC_APB2Periph_GPIOB

#define hc_echo_port   GPIOB
#define hc_echo_pin    GPIO_Pin_11
#define hc_echo_clk    RCC_APB2Periph_GPIOB

void hc_sr04_io_init(void);
void hc_sr04_timer4_init(int arr, int psc);
int get_hc_sr04_distance();
void drawScanLine(float angle, uint16_t dist);
void hc_sr04_follow_init(void);
void hc_sr04_follow_perform(void);	

#endif