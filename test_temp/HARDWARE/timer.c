#include "timer.h"

static u8 delay_us = 0;
#define ENCODER_TIM_PERIOD (u16)(65535)   //103�Ķ�ʱ����16λ 2��16�η������65536

//��ʱ��3��us����ʱ���������IIC
//void timer3_init(void)
//{
//    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
//    NVIC_InitTypeDef          NVIC_InitTypeD;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
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

