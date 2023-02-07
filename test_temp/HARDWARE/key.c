#include "key.h"

int key_down_times = 0;
int key_down_second_times = 0;
int key_up_times = 0;
int key_state = 0;
int key_first_down = 0;
int key_second_down = 0;


//PC15--电路上拉
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitST;

	GPIO_InitST.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitST.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitST);
}


void check_key_sta(void)
{
    unsigned tmp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15); //按下为0，松开为1
	
    if (tmp == 0) //按下
    {
        key_down_times++;
		
		if (key_down_times >= 30)
		{
		  key_first_down = 1; //按键按下标志
		}
		
		if(key_up_times > 10 && key_up_times < 200 && key_first_down == 1) //按键按下，松开时间在10--200ms又按下按键
		{ 
		  key_second_down = 1;
		  key_down_second_times++;
		}
    }

    if (tmp == 1 && key_first_down == 1) //上一刻按键按下，现在松开
    {
        key_up_times++;  //按键松开计时
		
		if (key_down_times < 1000 && key_up_times > 200)
		{
		  key_state = KEY_SHORT;
		  key_down_times = 0;
          key_up_times = 0;
		  key_down_second_times = 0;
		  key_first_down = 0;
		}
		else if(key_second_down == 1 && key_down_second_times > 30 && key_down_second_times < 500) //按下-松开--按下--松开
		{
		  key_state = KEY_DOUBLE;
		  key_down_times = 0;
          key_up_times = 0;
		  key_down_second_times = 0;
		  key_second_down = 0;
		  key_first_down = 0;
		}
    }

	if ((key_down_times >= 1000) && (key_up_times > 20))
    {
        key_state = KEY_LONG;
        key_down_times = 0;
        key_up_times = 0;
		key_down_second_times = 0;
		key_first_down = 0;
    }
	
}

int get_key_sta(void)
{
  int tmp = key_state;
  key_state = 0;
  return tmp;
}