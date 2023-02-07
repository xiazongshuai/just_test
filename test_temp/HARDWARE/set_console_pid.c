#include "set_console_pid.h"
#include "wheel_pid.h"
#include "follow_wall_pid.h"

#define set_wheel_pid 
//#define set_followall_pid 
//#define set_follow_pid

/****** ��������PID ******* ������ǽPID *****���ø���PID*******/

// ���õ�ǰ��Ŀ��ֵ
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

// ��ȡ��ǰ��Ŀ��ֵ
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

//����PID
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






