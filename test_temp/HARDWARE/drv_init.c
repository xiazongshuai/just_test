#include "drv_init.h"

void drv_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
	                     RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|
                         RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF| 
                    	 RCC_APB2Periph_GPIOG, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //轮子PWM
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //轮子码盘，定时器编码器模式
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //轮子码盘，定时器编码器模式
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
  key_init();
  led_init();
  uart1_init(115200);
//  uart2_init(115200);
  //app_uart2_init();
//  timer3_init();
//  soft_iic_io_init();
//  OLED_Init();
	
	protocol_init();
	
  hc_sr04_io_init();
  hc_sr04_timer4_init(100-1,72-1);//100us
  hc_sr04_follow_init();

  systick_ms(10); //不加这个延时有个轮子不能转动
  encoder_init_timer2(65535, 0);
  encoder_init_timer3(65535, 0);

  motor_pwm_init(7199,0); //10k
	
  motor_dir_io_init();
  wheel_init();
}

	
/*****************修改蓝牙的默认通信波特率以及蓝牙默认的名字******************/
void app_uart2_init(void)
{
	Uart2SendStr("AT\r\n");
	Uart2SendStr("AT+NAMEBigFish\r\n");//发送蓝牙模块指令--设置名字
	systick_ms(100);
	Uart2SendStr("AT+ROLE0\r\n");//发送蓝牙模块指令--设置模块角色
	systick_ms(100);
    Uart2SendStr("AT+PIN1234\r\n");//发送蓝牙模块指令--设置配对密码
	systick_ms(100);	
	Uart2SendStr("AT+BAUD8\r\n"); //发送蓝牙模块指令,将波特率设置成115200   默认波特率为9600，需注意第一次使用该模块时串口波特率的设置
	systick_ms(100);		
	uart2_init(115200);
}
/*****************************************************************************/	