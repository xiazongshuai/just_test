#include "stm32f10x.h"
#include "stdbool.h"
#include "drv_init.h"
#include "virtualosc.h"
#include "handle_console_pid.h"


int time_1ms = 0;
int time_5ms = 0;
int time_100ms = 0;

int key_value = 0;

int test_data;
int target_speed = 1500;
int main(void)
{	
  systick_init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  drv_init();

  printf("main_begin\r\n");

  time_1ms = millis();
  time_5ms = millis();
	time_100ms = millis();
	
	//set_wheel_speed(100, 100);
	set_computer_value(SEND_TARGET_CMD, CURVES_CH1,  &target_speed, 1); 
  while(1)
  {
	  if(millis() - time_100ms > 50)
	  {
	    time_100ms = millis();
//			hc_sr04_follow_perform();
//			wheel_tick();
			
			if(test_data < 2000)
			{
			  test_data += 50;
			}
			else
			{
			  test_data -= 50;
			}
			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &test_data, 1);
			printf("test = %d\r\n",test_data);
	  }
//    hc_sr04_follow_perform();
//	  systick_ms(1000);
  } 
}
