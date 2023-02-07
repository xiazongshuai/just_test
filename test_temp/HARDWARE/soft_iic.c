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

	/*STM32F103C8T6оƬ��Ӳ��I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //GPIO_Mode_AF_OD;//Ӳ��I2C���뿪©���
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

//ע�⣬�������������ݹ����У���SCL���ڸߵ�ƽʱ��SDA�ϵ�ֵ��Ӧ�ñ仯����ֹ�������һ��ֹͣ����
//����ÿ����һ��������󶼽�SCL����-->ǯס����  ��ÿ�ʼ��������ǰҲ��SCL����

U8 I2C_Start(void)
{
	SDA_Out();
	SDA_H;
	SCL_H;
	I2C_delay();
	SDA_L;
	I2C_delay(); //��ʱ�ȶ�
	SCL_L;    //����SCL��ǯ��IIC����
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


//��������Ӧ���ź�ACK
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

//����������Ӧ���ź�NACK
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

//�ȴ�Ӧ���źŵ���
//ÿ��������ӻ�������һ���ֽڵ����ݣ�����������Ҫ�ȴ��ӻ�����һ��Ӧ���źţ���ȷ�ϴӻ��Ƿ�ɹ����յ�������
//**ÿ����һ���ֽڣ�8��bit��**��һ���ֽڴ����8��ʱ�Ӻ�ĵھŸ�ʱ���ڼ䣬�������������ݺ�����һ��ACKӦ���źŸ����������������ܽ������ݴ���
U8 I2C_WaitAck(void) 
{
	U8 timeout = 0;
	SCL_L;
	SDA_H;
	I2C_delay();
	SDA_In(); 
	SCL_H;
	I2C_delay();
	while(SDA_Read)  //Ӧ��0  ��Ӧ��1
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

//����ack = 1  ��ȡ������Ҫ��Ӧ
//��ʾ����Ҫ��ȡ���ݣ��������Ҫ�ٶ�ȡ��������Ҫ����Ӧ
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

/*�������յ�Ӧ���źź�����Ҫ�ı�ͨ��ģʽ(�������ɷ��ͱ�Ϊ���գ��ӻ����ɽ��ձ�Ϊ����)
�����������·���һ����ʼstart�źţ�
Ȼ������ŷ���һ���ӻ���ַ��ע���ʱ�õ�ַ�ĵ�8λΪ1���������������óɽ���ģʽ��ʼ��ȡ����*/
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

U8 SequenceRead(U8 Address, U8 REG_Address, U8 *data, U32 size)  //���ϸ�ʽ--��дһ�����
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


U8 SequenceWrite(U8 addr, U8 reg, U8 len, const U8 *buf) //ֻ����д��һ��ģʽ
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