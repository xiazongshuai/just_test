#include "led.h"

int led_time = 0;


LED_DEV tick_led = led_off;

//PC14--µÕµÁ∆Ωµ„¡¡
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitST;

	GPIO_InitST.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitST.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitST.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitST);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
}

void set_led_sta(LED_DEV led_sta)
{
	switch(led_sta)
	{
		case led_on:
			GPIO_ResetBits(GPIOC, GPIO_Pin_14);
			break;
		case led_off:
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			break;
		case led_blink:
			tick_led = led_blink;
		    led_time = 0;
			break;
		case led_breath:
			tick_led = led_breath;
		    led_time = 0;
			break;
		default:
			break;
	}
}

int led_breath_on = 30;
int led_breath_flag = 0;
int led_breath_switch_time = 30;
int test_flag = 0;

void led_tick(void)
{
  if(tick_led == led_blink)
  {
    led_time++;
	if(led_time == 300)
	{
	  GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	}
	if(led_time == 600)
	{
	  GPIO_SetBits(GPIOC, GPIO_Pin_14);
	  led_time = 0;
	}
  }
  if(tick_led == led_breath)
  {
	led_time++;
    if(led_time < led_breath_on)
	{
	  GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	}
	else
	{
	  GPIO_SetBits(GPIOC, GPIO_Pin_14);
	}
	
	
    if(led_time == led_breath_switch_time)
	{
	  led_time = 0;
	  led_breath_flag = 1;
	}
	if(led_breath_flag == 1)
	{
	  led_breath_flag = 0;
		if(test_flag == 1)
		{
		  led_breath_on++;
			if(led_breath_on == 40)
			{
			 test_flag = 0;
			}
		}
		else
		{
		  led_breath_on--;
			if(led_breath_on == 0)
			{
			  test_flag = 1;
			}
		}
	}
  }
}