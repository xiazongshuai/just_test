#include "dma.h"

char uart1_data[20];

/*
* 串口1作为调试打印串口，使用其DMA发送功能
* DMA1_Channel4--usatr1_tx--PA9   DMA1_Channel5--usatr1_rx--PA10
*/
void uart1_dma_init(u32 bound)
{
	DMA_InitTypeDef   DMA_InitTypeD;
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	//DMA初始化
	DMA_InitTypeD.DMA_BufferSize = sizeof(uart1_data)/sizeof(uart1_data[0]);
	DMA_InitTypeD.DMA_DIR = DMA_DIR_PeripheralSRC;  //外设是源
	DMA_InitTypeD.DMA_M2M = DMA_M2M_Disable;
	
	DMA_InitTypeD.DMA_MemoryBaseAddr = (uint32_t)&uart1_data[0];
	DMA_InitTypeD.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitTypeD.DMA_MemoryInc =  DMA_MemoryInc_Enable;
	DMA_InitTypeD.DMA_Mode = DMA_Mode_Circular;
	
	DMA_InitTypeD.DMA_PeripheralBaseAddr = USART1->DR;
	DMA_InitTypeD.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitTypeD.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitTypeD.DMA_Priority = DMA_Priority_High;
    
	DMA_Init(DMA1_Channel4, &DMA_InitTypeD);  //DMA1_Channel4--usatr1_tx   DMA1_Channel5--usatr1_rx
	DMA_Cmd(DMA1_Channel4, ENABLE);
}


/*
* 串口2是蓝牙模块串口
* usart3：DMA1_Channel7--usatr1_tx--PA2    DMA1_Channel6--usatr1_rx--PA3
*/
void uart2_dma_init(u32 bound)
{
     //GPIO端口设置
	DMA_InitTypeDef   DMA_InitTypeD;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
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

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure);  
	
	USART_DMACmd(USART2, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);  //发送、接收都使用DMA
	USART_Cmd(USART2, ENABLE);    

	//DMA初始化--RX
	DMA_InitTypeD.DMA_BufferSize = sizeof(uart1_data)/sizeof(uart1_data[0]);
	DMA_InitTypeD.DMA_DIR = DMA_DIR_PeripheralSRC;  //外设是源
	DMA_InitTypeD.DMA_M2M = DMA_M2M_Disable;

	DMA_InitTypeD.DMA_MemoryBaseAddr = (uint32_t)&uart1_data[0];
	DMA_InitTypeD.DMA_PeripheralBaseAddr = USART2->DR;
	DMA_InitTypeD.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitTypeD.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitTypeD.DMA_MemoryInc =  DMA_MemoryInc_Enable;
	DMA_InitTypeD.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitTypeD.DMA_Mode = DMA_Mode_Circular;
	DMA_InitTypeD.DMA_Priority = DMA_Priority_High;

	DMA_Init(DMA1_Channel6, &DMA_InitTypeD);  //DMA1_Channel6--usatr1_rx
	DMA_Cmd(DMA1_Channel6, ENABLE);
	
	//DMA初始化--TX
	DMA_InitTypeD.DMA_BufferSize = sizeof(uart1_data)/sizeof(uart1_data[0]);
	DMA_InitTypeD.DMA_DIR = DMA_DIR_PeripheralSRC;  //外设是源
	DMA_InitTypeD.DMA_MemoryBaseAddr = (uint32_t)&uart1_data[0];
	DMA_InitTypeD.DMA_PeripheralBaseAddr = USART1->DR;
	
	DMA_Init(DMA1_Channel7, &DMA_InitTypeD);  //DMA1_Channel7--usatr1_tx 
	DMA_Cmd(DMA1_Channel7, ENABLE);
}


/*
* 串口3是板子空出的一组串口，作为两块板子通信接口
* usart3：DMA1_Channel2--usatr3_tx--PB10    DMA1_Channel3--usatr3_rx--PB11
*/
void uart3_dma_init(u32 bound)
{
     //GPIO端口设置
	DMA_InitTypeDef   DMA_InitTypeD;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	//USART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART3_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure);  
	
	USART_DMACmd(USART3, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);  //发送、接收都使用DMA
	USART_Cmd(USART3, ENABLE);    

	//DMA初始化--RX
	DMA_InitTypeD.DMA_BufferSize = sizeof(uart1_data)/sizeof(uart1_data[0]);
	DMA_InitTypeD.DMA_DIR = DMA_DIR_PeripheralSRC;  //外设是源
	DMA_InitTypeD.DMA_M2M = DMA_M2M_Disable;

	DMA_InitTypeD.DMA_MemoryBaseAddr = (uint32_t)&uart1_data[0];
	DMA_InitTypeD.DMA_PeripheralBaseAddr = USART3->DR;
	DMA_InitTypeD.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitTypeD.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitTypeD.DMA_MemoryInc =  DMA_MemoryInc_Enable;
	DMA_InitTypeD.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitTypeD.DMA_Mode = DMA_Mode_Circular;
	DMA_InitTypeD.DMA_Priority = DMA_Priority_High;

	DMA_Init(DMA1_Channel6, &DMA_InitTypeD);  //DMA1_Channel6--usatr1_rx
	DMA_Cmd(DMA1_Channel6, ENABLE);
	
	//DMA初始化--TX
	DMA_InitTypeD.DMA_BufferSize = sizeof(uart1_data)/sizeof(uart1_data[0]);
	DMA_InitTypeD.DMA_DIR = DMA_DIR_PeripheralSRC;  //外设是源
	DMA_InitTypeD.DMA_MemoryBaseAddr = (uint32_t)&uart1_data[0];
	DMA_InitTypeD.DMA_PeripheralBaseAddr = USART1->DR;
	
	DMA_Init(DMA1_Channel7, &DMA_InitTypeD);  //DMA1_Channel7--usatr1_tx 
	DMA_Cmd(DMA1_Channel7, ENABLE);
}
	
	
	      