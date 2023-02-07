#include "set_console_pid.h"
#include "wheel_pid.h"
#include "follow_wall_pid.h"

#define set_wheel_pid 
//#define set_followall_pid 
//#define set_follow_pid

/****** 设置轮速PID ******* 设置沿墙PID *****设置跟随PID*******/

// 设置当前的目标值
void set_pid_target(float temp_val)
{
	#if defined(set_wheel_pid) 
  wheel_pid_l.setpoint = temp_val;    
	#endif
	
	#if defined(set_followall_pid)
  wall_wheel.setpoint = temp_val;    
	#endif
	
	#if defined(set_follow_pid)
  wall_wheel.setpoint = temp_val;    
	#endif
}

// 获取当前的目标值
float get_pid_target(void)
{
	#if defined(set_wheel_pid)
  return wheel_pid_l.setpoint;    
	#endif
	
	#if defined(set_followall_pid)
  return wall_wheel.setpoint;   
	#endif
	
	#if defined(set_follow_pid)
  return wall_wheel.setpoint;   
	#endif
}

//设置PID
void set_p_i_d(float p, float i, float d)
{
	#if defined(set_wheel_pid)
  wheel_pid_l.p = p;    
	wheel_pid_l.i = i;   
	wheel_pid_l.d = d;   
	#endif
	
	#if defined(set_followall_pid)
  wall_wheel.p = p;    
	wall_wheel.i = i;   
	wall_wheel.d = d;   
	#endif
	
	#if defined(set_follow_pid)
  wall_wheel.p = p;    
	wall_wheel.i = i;   
	wall_wheel.d = d;   
	#endif
}






