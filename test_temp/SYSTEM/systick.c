#include "systick.h"
#include "led.h"

volatile static u32 delay = 0;
volatile static u32 time = 0;

void systick_init()
{
  if(SysTick_Config(72000)) //72M 优先级最低  1ms
  {
     while(1)
	 {
	 
	 }		 
  }
  NVIC_SetPriority (SysTick_IRQn, 0); //将优先级调为最高
}


void systick_ms(u32 num)
{
   delay = num;
   while(0 != delay)
	{
	
	}
}
void systick_s(u32 num)
{
   systick_ms(num*1000);
}



u32 millis(void)
{
 return time;
}
void inctic(void)
{
 time++;
}
void SysTick_Handler()
{
  if(0 != delay)
   {
     delay--;
   }
   inctic();
   led_tick();
}
