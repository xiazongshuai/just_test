#include "usart.h"
#include "handle_console_pid.h"
#include <string.h>
#include "motor.h"

void uart1_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitTypeD;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	NVIC_InitTypeD.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitTypeD.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitTypeD.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeD);
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);                    
}

void uart2_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitTypeD;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	//USART2_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART2_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  NVIC_InitTypeD.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitTypeD.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitTypeD.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitTypeD.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitTypeD);
	
   //USART 初始化设置
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure);     
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);                   
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
  u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	  Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
    //USART_SendData(USART1, Res);		
		protocol_data_recv(&Res, 1);
	}
} 

void sendUartBytes(char * data, unsigned short datalen)
{
	for(int i=0; i<datalen; i++)
	{
	  USART_SendData(USART1, data[i]);
	}
}

//printf
int fputc(int ch, FILE *f)
{
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_SendData(USART1,(uint8_t)ch);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
  return ch;
}

void USART2_IRQHandler(void)
{
	int Uart2_Receive;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//接收中断标志位拉高
	{
		Uart2_Receive=USART_ReceiveData(USART2);//保存接收的数据
		//printf("uart2_rec = %c\r\n",Uart2_Receive);
    BluetoothCMD(Uart2_Receive);
	}
}


void Uart2SendByte(char byte)  
{
	USART_SendData(USART2, byte);       
	while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  	
}


void Uart2SendStr(char *str)
{
	u16 i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart2SendByte(*str++);
	printf("send ok\r\n");
}

void BluetoothCMD(int Uart_Receive)
{
	switch(Uart_Receive)
		{
			case '0'://停止
			set_wheel_pwm_speed(0, 0);
			break;
			
			case '1'://前进
			set_wheel_pwm_speed(-1000, -1000);	
			printf("xxx = %d %d\r\n",TIM1->CCR1,TIM1->CCR4);
			break;
			
			case '2'://后退
			printf("xxx = %d %d\r\n",TIM1->CCR1,TIM1->CCR4);
			set_wheel_pwm_speed(1000, 1000);	
			break;
			
			case '3'://左转
			set_wheel_pwm_speed(1000, -1000);	
			break;
			
			case '4'://右转
			set_wheel_pwm_speed(-1000, 1000);
			break;
			
			default://停止
			set_wheel_pwm_speed(0, 0);	
			break;
		}
}

