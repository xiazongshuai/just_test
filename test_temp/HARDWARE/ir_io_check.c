#include "ir_io_check.h"

/*
 �ֱ��ӿ��������������
  PB3--L    PB4--LM
  PA5--RM   PA4--R
  �յ�����صĺ����źţ�ģ������͵�ƽ��
  ������������ǵ͵�ƽ����������ɫʱ������ߵ�ƽ
*/

IR_IO ir_io_result;

void ir_io_init()
{
    GPIO_InitTypeDef GPIO_InitST;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitST.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitST.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitST);
	
	GPIO_InitST.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitST);
}


void ir_io_check()
{
  if(SET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) ir_io_result.left = 1;
  else ir_io_result.left = 0;
	
   if(SET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)) ir_io_result.mleft = 1;
  else ir_io_result.mleft = 0;
	
   if(SET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)) ir_io_result.mright = 1;
  else ir_io_result.mright = 0;
	
   if(SET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)) ir_io_result.right = 1;
  else ir_io_result.right = 0;
}
