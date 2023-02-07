#include "timer.h"

static u8 delay_us = 0;
#define ENCODER_TIM_PERIOD (u16)(65535)   //103的定时器是16位 2的16次方最大是65536

//定时器3做us级延时，用于软件IIC
//void timer3_init(void)
//{
//    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
//    NVIC_InitTypeDef          NVIC_InitTypeD;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
//    
//	NVIC_InitTypeD.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitTypeD.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitTypeD.NVIC_IRQChannelCmd = ENABLE;
//	
//	TIM_TimeBaseStructure.TIM_Period = 72-1; 
//	TIM_TimeBaseStructure.TIM_Prescaler = 1-1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
//	
//	
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
//	NVIC_Init(&NVIC_InitTypeD);
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	TIM_Cmd(TIM3, ENABLE);  		
//}

//void TIM3_IRQHandler(void)
//{
//   if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//	  {
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//		  if(0 != delay_us)
//		  {
//            delay_us--;		  
//		  }
//	  }
//}

//void timer3_delay_us(u8 num)
//{
//  delay_us = num;
//  while(0 != delay_us)
//   {
//	
//   }
//}

