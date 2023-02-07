#ifndef _FOLLOW_WALL_PID_H
#define _FOLLOW_WALL_PID_H

struct wall_pid_struct
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
extern struct wall_pid_struct wall_wheel;

void follow_work_follow_wall_init();
void follow_work_follow_wall_perform();

#endif