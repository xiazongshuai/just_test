注意软硬件IIC，引脚模式不一样
硬件IIC--GPIO_Mode_AF_OD;//I2C必须开漏输出
软件IIC--GPIO_Mode_Out_PP


问题：
轮速不受控制，打印、else if、TIM_SetCounter...都会影响
原因：
timer3_init();微妙级别延时对程序照成的莫名影响