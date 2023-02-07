#include "wheel_pid.h"
#include "motor.h"
#include "stdio.h"
#include "handle_console_pid.h"


/*****
减速比30:1  编码器单圈输出13个脉冲  减速后390  4倍频至1560
轮子直径65mm
330RPM
速度最大设置为1100mm/s
******/

struct wheel_pid_struct wheel_pid_l;
struct wheel_pid_struct wheel_pid_r;
struct wheel_pid_data wheel_data_l;
struct wheel_pid_data wheel_data_r;

void wheel_pid_data_init()
{
	wheel_data_l.type = 2;
	wheel_data_l.distance = 0;
	wheel_data_l.cur_encoder = 0;
	wheel_data_l.pre_encoder = 0;
	wheel_data_l.dif_encoder = 0;

	wheel_data_r.type = 1;
	wheel_data_r.distance = 0;
	wheel_data_r.cur_encoder = 0;
	wheel_data_r.pre_encoder = 0;
	wheel_data_r.dif_encoder = 0;
}

void wheel_pid_struct_init(struct wheel_pid_struct *P, float p, float i, float d)
{
	P->setpoint = 0;
	P->p = p;
	P->i = i;
	P->d = d;
	P->error = 0;
	P->increment = 0; 
	P->lasterror = 0; //上一次误差
	P->preverror = 0; //上上次误差
	P->uk = 0;
}


void get_dif_encoder(struct wheel_pid_data *wheel)
{
	//wheel->cur_encoder = read_encoder(wheel);
	//wheel->dif_encoder = wheel->cur_encoder - wheel->pre_encoder;
	//wheel->pre_encoder = wheel->cur_encoder;
	
	wheel->dif_encoder = read_encoder(wheel);
}

int speed_pid_control(struct wheel_pid_struct *p, float measual_speed)
{
	p->error = p->setpoint - measual_speed;
	p->increment =  p->p*(p->error - p->lasterror)
					+ p->i*p->error
					+ p->d*(p->error- 2*p->lasterror + p->preverror);
	p->preverror = p->lasterror;
	p->lasterror = p->error;
	p->uk += p->increment;
	return p->uk;
}

float speed[2] = {0, 0};
void wheel_speed_to_pid(struct wheel_pid_data *wheel_l,struct wheel_pid_data *wheel_r, float proportion) //proportion 码盘计数1，实际运行距离
{                                                                                                        //(3.1415*65) / 1560 = 0.13  一个脉冲计数，运行距离为0.13mm
	int left_speed = 0, right_speed = 0;
	int left_measual_speed = 0, right_measual_speed = 0;
	get_dif_encoder(wheel_l);
	get_dif_encoder(wheel_r);

	wheel_l->distance = wheel_l->dif_encoder * proportion;  //脉冲计数时采用了4倍频
	wheel_r->distance = wheel_r->dif_encoder * proportion;
  
	speed[0] = wheel_l->distance / (float)interval_time;  
	speed[1] = wheel_r->distance / (float)interval_time;
	
	//printf("speed = %2f %2f  %d  %d\r\n",speed[0],speed[1],wheel_l->distance,wheel_r->distance);
	
  left_measual_speed = speed[0] * 1000;   //速度转为mm/s
	right_measual_speed = speed[1] * 1000;
	
	//printf("measual_speed l r = %d %d \r\n",left_measual_speed,right_measual_speed);
	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &left_measual_speed, 1);  //传给上位机的数据
	
	left_speed = speed_pid_control(&wheel_pid_l,left_measual_speed);
	right_speed = speed_pid_control(&wheel_pid_r,right_measual_speed);
	
	set_wheel_pwm_speed(left_speed, right_speed);
	//printf("pwm = %d %d\r\n",left_speed,right_speed);
}


void wheel_init()
{
  wheel_pid_data_init();
  wheel_pid_struct_init(&wheel_pid_l,1.0,1.0,1.0);
  wheel_pid_struct_init(&wheel_pid_r,1.0,1.0,1.0);
}

void wheel_tick()
{
  wheel_speed_to_pid(&wheel_data_l,&wheel_data_r, CODER_FACTOR);
}

void set_wheel_speed(int left, int right)
{
	wheel_pid_l.setpoint = left;
	wheel_pid_r.setpoint = right;
}