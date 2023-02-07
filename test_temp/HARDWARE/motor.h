#ifndef _MOTOR_H
#define _MOTOR_H

#include "wheel_pid.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void motor_pwm_init(int arr, int psc);
void motor_dir_io_init();
void set_wheel_pwm_speed(int left, int right);

void encoder_init_timer2(int arr, int psc);
void encoder_init_timer3(int arr, int psc);
int read_encoder(struct wheel_pid_data *p);

void set_wheel_pwm(int left_pwm, int right_pwm);
#endif