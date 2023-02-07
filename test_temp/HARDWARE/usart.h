#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void uart1_init(u32 bound);					
void uart2_init(u32 bound);
void Uart2SendStr(char *str);
void Uart2SendByte(char byte);

void sendUartBytes(char * data, unsigned short datalen);
void BluetoothCMD(int Uart_Receive);
#endif