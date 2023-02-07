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
	int lasterror; //上一次误差
	int preverror; //上上次误差
	int uk;
};

struct wheel_pid_data
{
	int type;  //1左轮  2右轮
  int distance; 				//此次与上次采集时间段内行进的距离
	int cur_encoder;				//此次采集的码盘值
	int pre_encoder;				//上次采集的码盘值
	int dif_encoder;				//两次的差值:(cur_encoder -pre_encoder),表示:10ms走的距离mm
};

extern struct wheel_pid_struct wheel_pid_l;
extern struct wheel_pid_struct wheel_pid_r;
extern struct wheel_pid_data wheel_data_l;
extern struct wheel_pid_data wheel_data_r;

void wheel_init();
void wheel_tick();
void set_wheel_speed(int left, int right);
#endif