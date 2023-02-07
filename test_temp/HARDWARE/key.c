#include "key.h"

int key_down_times = 0;
int key_down_second_times = 0;
int key_up_times = 0;
int key_state = 0;
int key_first_down = 0;
int key_second_down = 0;


//PC15--��·����
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitST;

	GPIO_InitST.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitST.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitST);
}


void check_key_sta(void)
{
    unsigned tmp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15); //����Ϊ0���ɿ�Ϊ1
	
    if (tmp == 0) //����
    {
        key_down_times++;
		
		if (key_down_times >= 30)
		{
		  key_first_down = 1; //�������±�־
		}
		
		if(key_up_times > 10 && key_up_times < 200 && key_first_down == 1) //�������£��ɿ�ʱ����10--200ms�ְ��°���
		{ 
		  key_second_down = 1;
		  key_down_second_times++;
		}
    }

    if (tmp == 1 && key_first_down == 1) //��һ�̰������£������ɿ�
    {
        key_up_times++;  //�����ɿ���ʱ
		
		if (key_down_times < 1000 && key_up_times > 200)
		{
		  key_state = KEY_SHORT;
		  key_down_times = 0;
          key_up_times = 0;
		  key_down_second_times = 0;
		  key_first_down = 0;
		}
		else if(key_second_down == 1 && key_down_second_times > 30 && key_down_second_times < 500) //����-�ɿ�--����--�ɿ�
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