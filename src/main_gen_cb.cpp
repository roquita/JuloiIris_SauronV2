#include "main.h"

void wheel_at_start_cb()
{
    load_toogling_circulin();
}

void wheel_at_stop_cb()
{
    load_TurnOff_circulin();
}

void ptz_at_start_cb()
{
    load_toogling_circulin();
}

void ptz_at_stop_cb()
{
    load_TurnOff_circulin();
}

void ROS_at_transport_disconnected_cb()
{
    load_toogling_PilotYellow();
    wheel_stop();
}

void ROS_at_transport_connected_cb()
{
    load_TurnOff_PilotYellow();
}
void wheel_MotorFrontRight_at_fault()
{
    alarm_set_MotorFrontRight();
}
void wheel_MotorFrontRight_at_NoFault()
{
    alarm_clear_MotorFrontRight();
}
void wheel_MotorFrontLeft_at_fault()
{
    alarm_set_MotorFrontLeft();
}
void wheel_MotorFrontLeft_at_NoFault()
{
    alarm_clear_MotorFrontLeft();
}
void wheel_MotorBackRight_at_fault()
{
    alarm_set_MotorBackRight();
}
void wheel_MotorBackRight_at_NoFault()
{
    alarm_clear_MotorBackRight();
}
void wheel_MotorBackLeft_at_fault()
{
    alarm_set_MotorBackLeft();
}
void wheel_MotorBackLeft_at_NoFault()
{
    alarm_clear_MotorBackLeft();
}
void ptz_motor_at_fault()
{
    alarm_set_MotorPtz();
}
void ptz_motor_at_NoFault()
{
    alarm_clear_MotorPtz();
}
