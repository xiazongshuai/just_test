#include <stdio.h>
#include "follow_wall_pid.h"   //头文件使用"" 和 <> 查找的路径不同
#include "hc_sr04.h"
#include "wheel_pid.h"


struct wall_pid_struct wall_wheel;

#define follow_speed  300

struct follow_wall_data_struct
{
  int speed;
}follow_wall_data;

void wall_pid_init(struct wall_pid_struct *wheel_pid, float p, float i, float d, int setdis)
{
  wheel_pid->p = p;
  wheel_pid->i = i;
  wheel_pid->d = d;
  wheel_pid->setpoint = setdis;
  wheel_pid->error = 0;
  wheel_pid->lasterror = 0;
  wheel_pid->preverror = 0;
  wheel_pid->increment = 0.0;
  wheel_pid->uk = 0;
}

int wall_pid_control(struct wall_pid_struct *wheel_pid, int actual_dis)
{
  wheel_pid->error = wheel_pid->setpoint - actual_dis;
  wheel_pid->increment =  wheel_pid->p*(wheel_pid->error - wheel_pid->lasterror)
	                    + wheel_pid->i*wheel_pid->error
	                    + wheel_pid->d*(wheel_pid->error- 2*wheel_pid->lasterror + wheel_pid->preverror);
  wheel_pid->preverror = wheel_pid->lasterror;
  wheel_pid->lasterror = wheel_pid->error;
  wheel_pid->uk += wheel_pid->increment;
	
	if(wheel_pid->uk > 1000)
  {
    return 1000;
  }
	
  if(wheel_pid->uk < -1000)
  {
    return -1000;
  }
  return wheel_pid->uk;
}

void follow_work_follow_wall_init()
{
  wall_pid_init(&wall_wheel, 1.0f, 1.0f, 0.f, 5);
}

void follow_work_follow_wall_perform()
{
  int dis,uk_speed;
  int wheel_speed[2]={0,0};
  
  dis = get_hc_sr04_distance();
  uk_speed = wall_pid_control(&wall_wheel, dis);
  printf("uk_speed = %d\r\n",uk_speed);
	
//  wheel_speed[0] = follow_wall_data.speed - uk_speed;
//  wheel_speed[1] = follow_wall_data.speed + uk_speed;
	
  wheel_speed[0] = follow_speed - uk_speed*0.2;
  wheel_speed[1] = follow_speed + uk_speed*0.2;
	
  set_wheel_speed(wheel_speed[0],wheel_speed[1]);
}