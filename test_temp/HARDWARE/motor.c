#include "motor.h"
#include "wheel_pid.h"
/*
ת�ٿ���PWM��
	PWMA--PA11--TIM1_CH4
	PWMB--PA8--TIM1_CH1
	
�������AIN:
	AIN1  AIN2   ����
	PB13  PB12
	0     1      ��ת
	1     0      ��ת
    0     0      ֹͣ
	
	BIN1  BIN2   ����
	PB14  PB15   ����
	0     1      ��ת
	1     0      ��ת
    0     0      ֹͣ
	
���������AO/BO:
PA0  PA1  TIMER2
PA6  PA7  TIMER3
*/


/*
* ���ӵ��PWM��ʼ��
* arr���Զ���װֵ
* psc��ʱ��Ԥ��Ƶ��
* TIM1_PWM_Init(7199,0);//PWMƵ��=72000/(7199+1)=10Khz
*/
void motor_pwm_init(int arr, int psc)
{
	GPIO_InitTypeDef GPIO_InitST;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitST;
	TIM_OCInitTypeDef  TIM_OCInitST;
	
	//GPIO
	GPIO_InitST.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitST.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitST.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitST);
	
	//������ʱ��
	TIM_TimeBaseInitST.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitST.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitST.TIM_Period = arr;
	TIM_TimeBaseInitST.TIM_Prescaler = psc;
	TIM_TimeBaseInitST.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitST);
	
	//PWM
	TIM_OCInitST.TIM_OCMode = TIM_OCMode_PWM1;  //С��ccr��Ч
	TIM_OCInitST.TIM_OCPolarity = TIM_OCPolarity_High;  //��ЧΪ��
	TIM_OCInitST.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitST.TIM_Pulse = 0;
	
	TIM_OC4Init(TIM1, &TIM_OCInitST);
	TIM_OC1Init(TIM1, &TIM_OCInitST);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��--ע��ô�	
	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1Ԥװ��ʹ��
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);
}


/*
* ����PWMռ�ձ�  ��CCR�Ĵ���ֵ�Ƚ�
*/
void set_wheel_pwm(int left_pwm, int right_pwm)
{
  TIM_SetCompare1(TIM1, left_pwm);
  TIM_SetCompare4(TIM1, right_pwm);
}

/*
* ���ӷ������ų�ʼ��
*/
void motor_dir_io_init()
{
	GPIO_InitTypeDef GPIO_InitST;
	
	GPIO_InitST.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitST.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitST.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitST);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

typedef enum
{
	left_motor_front = 0,
	left_motor_back,
	left_motor_stop,
	right_motor_front,
	right_motor_back,
	right_motor_stop,
}DEV_MOTOR_NAME;

typedef struct motor_dir
{
  unsigned motor_sta;
  unsigned in1_sta;
  unsigned in2_sta;
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin_in1;
  uint16_t GPIO_Pin_in2;
}MOTOR_DIR;

MOTOR_DIR DEV_MOTOR_DIR[] = 
{
  {right_motor_front,0, 1, GPIOB, GPIO_Pin_13, GPIO_Pin_12},
  {right_motor_back, 1, 0, GPIOB, GPIO_Pin_13, GPIO_Pin_12},
  {right_motor_stop, 0, 0, GPIOB, GPIO_Pin_13, GPIO_Pin_12},
  {left_motor_front, 0, 1, GPIOB, GPIO_Pin_14, GPIO_Pin_15},
  {left_motor_back,  1, 0, GPIOB, GPIO_Pin_14, GPIO_Pin_15},
  {left_motor_stop,  0, 0, GPIOB, GPIO_Pin_14, GPIO_Pin_15},
};


/*
* �������ӷ������
*/
void motor_dir_control(DEV_MOTOR_NAME motor_sta)
{
  for(int i = 0; i < sizeof(DEV_MOTOR_DIR)/sizeof(DEV_MOTOR_DIR[0]); i++)
	{
	  if(DEV_MOTOR_DIR[i].motor_sta == motor_sta)
	  {
	    if(DEV_MOTOR_DIR[i].in1_sta == 0)
		{
		  GPIO_ResetBits(DEV_MOTOR_DIR[i].GPIOx,  DEV_MOTOR_DIR[i].GPIO_Pin_in1);
		}
		else
		{
		  GPIO_SetBits(DEV_MOTOR_DIR[i].GPIOx,  DEV_MOTOR_DIR[i].GPIO_Pin_in1);
		}
		
		 if(DEV_MOTOR_DIR[i].in2_sta == 0)
		{
		  GPIO_ResetBits(DEV_MOTOR_DIR[i].GPIOx,  DEV_MOTOR_DIR[i].GPIO_Pin_in2);
		}
		else
		{
		  GPIO_SetBits(DEV_MOTOR_DIR[i].GPIOx,  DEV_MOTOR_DIR[i].GPIO_Pin_in2);
		}
	  }
	}
}


/*
 �����PWMֵ����Ϊ�����趨��������ҪתΪ�������뺯��
*/
int myabs(int tmp)
{
  if(tmp < 0)
	  return -tmp;
  else
	  return tmp;
}


/*
* ��ʵ�ʵ�PWMֵȥ�������ӵ�ת�� 
*/
void set_wheel_pwm_speed(int left, int right)
{
  if(left > 0)
  {
    motor_dir_control(left_motor_front);
  }
  if(left < 0)
  {
    motor_dir_control(left_motor_back);
  }
  if(left == 0)
  {
    motor_dir_control(left_motor_stop);
  }
  
  
  if(right > 0)
  {
    motor_dir_control(right_motor_front);
  }
  if(right < 0)
  {
    motor_dir_control(right_motor_back);
  }
  if(right == 0)
  {
    motor_dir_control(right_motor_stop);
  }
  
  set_wheel_pwm(myabs(left),myabs(right));
}




//��ʱ��������ģʽ--���̼������ڼ����ٶ�
void encoder_init_timer2(int arr, int psc)
{
  GPIO_InitTypeDef   GPIO_InitST;
  TIM_ICInitTypeDef  TIM_ICInitST;  
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseST; 
	
  GPIO_InitST.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	
  GPIO_InitST.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitST);					  
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseST);
  TIM_TimeBaseST.TIM_Prescaler = psc; 
  TIM_TimeBaseST.TIM_Period = arr; 
  TIM_TimeBaseST.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseST.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseST);
	
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitST);
  TIM_ICInitST.TIM_ICFilter = 10;
  TIM_ICInit(TIM2, &TIM_ICInitST);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM2,0);
  TIM_Cmd(TIM2, ENABLE); 
}

void encoder_init_timer3(int arr, int psc)
{
  GPIO_InitTypeDef   GPIO_InitST;
  TIM_ICInitTypeDef  TIM_ICInitST;  
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseST;
	
  GPIO_InitST.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	
  GPIO_InitST.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitST);					    
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseST);
  TIM_TimeBaseST.TIM_Prescaler = psc; // Ԥ��Ƶ�� 
  TIM_TimeBaseST.TIM_Period = arr; //�趨�������Զ���װֵ
  TIM_TimeBaseST.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseST.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseST);
	
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
  TIM_ICStructInit(&TIM_ICInitST);
  TIM_ICInitST.TIM_ICFilter = 10;
  TIM_ICInit(TIM3, &TIM_ICInitST);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM3,0);
  TIM_Cmd(TIM3, ENABLE); 
}

/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int read_encoder(struct wheel_pid_data *p)
{
   int encoder;    
   switch(p->type)
	 {
	   case 1:  encoder= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
	   case 2:  encoder= -(short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	 //���������ǶԳƵģ���һ���������£�����һ����Ҫȡ��'-'
	   default: encoder=0;break;
	 }
	 //printf("cur = %d %d\r\n",p->type,encoder);
	return encoder;
}
