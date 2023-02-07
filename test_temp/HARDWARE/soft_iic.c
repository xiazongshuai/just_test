#if 1

#include "soft_iic.h"
#include "timer.h"

void timer3_delay_us(u8 num)
{
  
}

#define I2C_delay() timer3_delay_us(1)

void soft_iic_io_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	/*STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //GPIO_Mode_AF_OD;//硬件I2C必须开漏输出
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

//注意，在正常传输数据过程中，当SCL处于高电平时，SDA上的值不应该变化，防止意外产生一个停止条件
//所以每处理一个数据最后都将SCL拉低-->钳住总线  最好开始处理数据前也将SCL拉低

U8 I2C_Start(void)
{
	SDA_Out();
	SDA_H;
	SCL_H;
	I2C_delay();
	SDA_L;
	I2C_delay(); //延时稳定
	SCL_L;    //拉低SCL，钳主IIC总线
	return 1;
}


void I2C_Stop(void)
{
	SCL_L;
	SDA_Out();
	SDA_L; 
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_L;
}


//主机产生应答信号ACK
void I2C_Ack(void)
{
	SCL_L;
	SDA_Out();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
}

//主机不产生应答信号NACK
void I2C_NoAck(void)
{
	SCL_L;
	SDA_Out();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
}

//等待应答信号到来
//每当主机向从机发送完一个字节的数据，主机总是需要等待从机给出一个应答信号，以确认从机是否成功接收到了数据
//**每发送一个字节（8个bit）**在一个字节传输的8个时钟后的第九个时钟期间，接收器接收数据后必须回一个ACK应答信号给发送器，这样才能进行数据传输
U8 I2C_WaitAck(void) 
{
	U8 timeout = 0;
	SCL_L;
	SDA_H;
	I2C_delay();
	SDA_In(); 
	SCL_H;
	I2C_delay();
	while(SDA_Read)  //应答0  非应答1
	{
		timeout++;
		if (timeout > 250) 
		{
			I2C_Stop();
			return 0;
		}
	}
	SCL_L; 
	return 1;
}


void I2C_WriteByte(U8 txd) 
{
	U8 t;
	SCL_L;
	SDA_Out();								
	for (t = 0; t < 8; t++) 
	{
		if ((txd & 0x80) >> 7) 
		{
			SDA_H;
		}
		else
			SDA_L;

		txd <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
		SCL_L;
	}
}

//参数ack = 1  读取数据需要响应
//表示还需要读取数据，如果不需要再读取数据则不需要再响应
U8 I2C_ReadByte(U8 ack)
{
	U8 i, receive = 0;
	SCL_L;
	SDA_In();	
	for (i = 0; i < 8; i++) 
	{
		SCL_L;
		I2C_delay();
		SCL_H;
		receive <<= 1;
		I2C_delay();
		if (SDA_Read)
			receive++;
	}
	SCL_L;
	if (!ack)
		I2C_NoAck(); 
	else
		I2C_Ack();	
	return receive; 
}


U8 Single_Write(U8 Address, U8 REG_Address, U8 REG_data) 
{
	if (!I2C_Start())
		return 0;
	I2C_WriteByte(Address); 
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return 1;
	}
	I2C_WriteByte(REG_Address);
	I2C_WaitAck();
	I2C_WriteByte(REG_data);
	I2C_WaitAck();
	I2C_Stop();
	return 0;
}

/*当主机收到应答信号后，主机要改变通信模式(主机将由发送变为接收，从机将由接收变为发送)
所以主机重新发送一个开始start信号，
然后紧跟着发送一个从机地址，注意此时该地址的第8位为1，表明将主机设置成接收模式开始读取数据*/
U8 Single_Read(U8 Address, U8 REG_Address, U8 *data)
{
	U8 REG_data;
	if (!I2C_Start())
		return 0;
	I2C_WriteByte(Address); 
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return 1;
	}
	I2C_WriteByte((uint8_t)REG_Address); 
	I2C_WaitAck();
	
	I2C_Start();
	I2C_WriteByte(Address + 1);
	I2C_WaitAck();

	REG_data = I2C_ReadByte(0);
	I2C_NoAck();
	I2C_Stop();

	*data = REG_data;
	return 0;
}

U8 SequenceRead(U8 Address, U8 REG_Address, U8 *data, U32 size)  //复合格式--读写一起存在
{
	U32 i = 0;

	if (!I2C_Start())
		return 1;			
	I2C_WriteByte(Address); 
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return 1; 
	}
	I2C_WriteByte(REG_Address); 
	I2C_WaitAck();

	I2C_Start();
	I2C_WriteByte(Address + 1); 
	I2C_WaitAck();

	if (size > 1)
	{
		for (i = 0; i < size - 1; i++)
		{
			data[i] = I2C_ReadByte(1);
		}
		data[size - 1] = I2C_ReadByte(0);
	}
	else
	{
		data[0] = I2C_ReadByte(0);
	}

	I2C_NoAck();
	I2C_Stop();

	return 0;
}


U8 SequenceWrite(U8 addr, U8 reg, U8 len, const U8 *buf) //只存在写这一种模式
{
	U8 i;
	I2C_Start();
	I2C_WriteByte(addr);					
	if (!I2C_WaitAck()) 
	{
		I2C_Stop();
		return 1;
	}
	I2C_WriteByte(reg); 
	I2C_WaitAck();		
	for (i = 0; i < len; i++)
	{
		I2C_WriteByte(buf[i]); 
		if (!I2C_WaitAck())	   
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_Stop();
	return 0;
}

#endif