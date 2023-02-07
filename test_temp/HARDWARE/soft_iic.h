#ifndef __SOFT_I2C_H
#define	__SOFT_I2C_H

#include "stm32f10x.h"

//PG6--SCL
//PG7--SDA

#define U8  unsigned char
#define U32 unsigned int
	
#define IIC_SCL_PORT   GPIOG
#define IIC_SCL_PIN    GPIO_Pin_6

#define IIC_SDA_PORT  GPIOG
#define IIC_SDA_PIN   GPIO_Pin_7

#define SDA_In()  {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=8<<(4*7);}  
#define SDA_Out() {GPIOG->CRL&=0X0FFFFFFF;GPIOG->CRL|=3<<(4*7);} 

	 
#define SCL_H   GPIO_SetBits(IIC_SCL_PORT, IIC_SCL_PIN);
#define SCL_L   GPIO_ResetBits(IIC_SCL_PORT, IIC_SCL_PIN);

#define SDA_H   GPIO_SetBits(IIC_SDA_PORT, IIC_SDA_PIN);
#define SDA_L   GPIO_ResetBits(IIC_SDA_PORT, IIC_SDA_PIN);

#define SDA_Read    GPIO_ReadInputDataBit(IIC_SDA_PORT, IIC_SDA_PIN)

U8 Single_Write(U8 Address, U8 REG_Address, U8 REG_data);
U8 SequenceWrite(U8 addr, U8 reg, U8 len, const U8 *buf);
void soft_iic_io_init();
#endif
