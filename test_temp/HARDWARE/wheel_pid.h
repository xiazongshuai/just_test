#ifndef WHEEL_PID_H
#define WHEEL_PID_H

#define CODER_FACTOR 0.13
#define interval_time 50; //ms

extern struct wheel_pid_struct wheel_pid_l;
extern struct wheel_pid_struct wheel_pid_r;
extern struct wheel_pid_data wheel_data_l;
extern struct wheel_pid_data wheel_data_r;

struct wheel_pid_struct
{
  float setpoint;
	float p;
	float i;
	float d;
	int error;
	float increment; 
	int lasterror; //��һ�����
	int preverror; //���ϴ����
	int uk;
};

struct wheel_pid_data
{
	int type;  //1����  2����
  int distance; 				//�˴����ϴβɼ�ʱ������н��ľ���
	int cur_encoder;				//�˴βɼ�������ֵ
	int pre_encoder;				//�ϴβɼ�������ֵ
	int dif_encoder;				//���εĲ�ֵ:(cur_encoder -pre_encoder),��ʾ:10ms�ߵľ���mm
};

extern struct wheel_pid_struct wheel_pid_l;
extern struct wheel_pid_struct wheel_pid_r;
extern struct wheel_pid_data wheel_data_l;
extern struct wheel_pid_data wheel_data_r;

void wheel_init();
void wheel_tick();
void set_wheel_speed(int left, int right);
#endif