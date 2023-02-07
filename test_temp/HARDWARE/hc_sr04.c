#include <math.h>
#include "hc_sr04.h"
#include "timer.h"
#include "oled.h"
#include "systick.h"
#include <stdio.h>
#include "motor.h"
#include "virtualosc.h"

int hc_time = 0; //中断溢出时间

void hc_sr04_io_init(void)
{
  GPIO_InitTypeDef  GPIO_InitTypeD;
  RCC_APB2PeriphClockCmd(hc_tring_clk|hc_echo_clk, ENABLE);
		
  GPIO_InitTypeD.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitTypeD.GPIO_Pin = hc_tring_pin;
  GPIO_InitTypeD.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(hc_tring_port, &GPIO_InitTypeD);
	
  GPIO_InitTypeD.GPIO_Mode = GPIO_Mode_IPD;//GPIO_Mode_IN_FLOATING;
  GPIO_InitTypeD.GPIO_Pin = hc_echo_pin;
  GPIO_Init(hc_echo_port, &GPIO_InitTypeD);
}

void hc_sr04_timer4_init(int arr, int psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitT;
  NVIC_InitTypeDef         NVIC_InitTS;	

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
  NVIC_InitTS.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitTS.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitTS.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitTS.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitTS);
	
  TIM_TimeBaseInitT.TIM_Period = arr;
  TIM_TimeBaseInitT.TIM_Prescaler = psc;
  TIM_TimeBaseInitT.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitT.TIM_ClockDivision = TIM_CKD_DIV1;
 // TIM_TimeBaseInitT.TIM_RepetitionCounter = 0;
	
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitT);
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM4, ENABLE);	
}

void TIM4_IRQHandler() //100us触发一次
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
	   hc_time++;
	   TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

void hc_sr04_delay_us(int num)
{
  while(72*num--)
	{
	
	}
}

//测试距离=（高电平时间*声速（340M/S））/2
int get_hc_sr04_distance()
{
  float dis = 0;
  TIM_Cmd(TIM4,ENABLE);
  GPIO_SetBits(hc_tring_port, hc_tring_pin);
  hc_sr04_delay_us(20); //最少10us的高电平
	//systick_ms(1);
  GPIO_ResetBits(hc_tring_port, hc_tring_pin);
	
  while(GPIO_ReadInputDataBit(hc_echo_port, hc_echo_pin) == RESET);
  TIM4->CNT = 0;
  hc_time = 0;
  while(GPIO_ReadInputDataBit(hc_echo_port, hc_echo_pin) == SET);
  TIM_Cmd(TIM4,DISABLE); 
  dis = (float)(((float)(TIM4->CNT + hc_time*100)/2) /1000000) * 340000; //mm

 // printf("time = %d %d\r\n",TIM_GetCounter(TIM4),hc_time);
 // printf("dis = %f \r\n",dis);
  return dis;
}


#define sr04_follow_speed  100

//超声波距离跟随--PID
typedef struct follow_pid_struct
{
  float p;
	float i;
	float d;
	int dis;
	int set_dis;
	int error;
	int allow_error;
	float increment; 
	int lasterror; //上一次误差
	int preverror; //上上次误差
	int uk;
}FOLLOW_PID;

FOLLOW_PID follow_pid;
FOLLOW_PID *p_follow;

void hc_sr04_follow_pid_init(float p, float i, float d, int setdis, int allow_error)
{
  follow_pid.p = p;
	follow_pid.i = i;
	follow_pid.d = d;
	follow_pid.dis = 0;
	follow_pid.set_dis = setdis;
	follow_pid.error = 0;
	follow_pid.allow_error = allow_error;
	follow_pid.increment = 0;
	follow_pid.lasterror = 0;
	follow_pid.preverror = 0;
	follow_pid.uk = 0;
}

int hc_sr04_follow_pid_control(int actual_dis)
{
  follow_pid.error = follow_pid.set_dis - actual_dis;
  follow_pid.increment =  follow_pid.p*(follow_pid.error - follow_pid.lasterror)
	                    + follow_pid.i*follow_pid.error
	                    + follow_pid.d*(follow_pid.error- 2*follow_pid.lasterror + follow_pid.preverror);
  follow_pid.preverror = follow_pid.lasterror;
  follow_pid.lasterror = follow_pid.error;
  follow_pid.uk += follow_pid.increment;
  if(follow_pid.uk > 1000)
  {
    return 1000;
  }
	
  if(follow_pid.uk < -1000)
  {
    return -1000;
  }
  return follow_pid.uk;
}

void hc_sr04_follow_init(void)
{
  hc_sr04_follow_pid_init(5.0,0.0,0.0,150,10); //距离单位是mm
}

int add;
float lab_test = 0;

void hc_sr04_follow_perform(void)
{
  follow_pid.dis = get_hc_sr04_distance();
  add = hc_sr04_follow_pid_control(follow_pid.dis);
	
  //printf("add = %d  %d\r\n",follow_pid.dis,add);
	//printf("%d \r\n",follow_pid.dis);
	
	lab_test = follow_pid.dis;
	updateVirtualOSC((char *)&lab_test,sizeof(lab_test));
	
	add = add*0.4;
	
  if(follow_pid.error < -follow_pid.allow_error)  //比设定距离远，前进
  {
   set_wheel_speed(sr04_follow_speed+add, sr04_follow_speed+add);
  }
  else if(follow_pid.error > follow_pid.allow_error) //比设定距离近，后退
  {
   set_wheel_speed(-sr04_follow_speed-add, -sr04_follow_speed-add);
  }
  else
  {
   set_wheel_speed(0, 0);	
  }
} 