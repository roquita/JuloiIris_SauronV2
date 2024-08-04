#pragma once

#include "Arduino.h"
#include "app/interval/interval.h"
#include "app/load/load.h"
#include "app/ros/ros.h"
#include "app/sensor/distance.h"
#include "app/sensor/temphumi.h"
#include "app/screen/screen.h"
#include "app/wheel/wheel.h"
#include "app/ptz/ptz.h"
#include "app/alarm/alarm.h"
#include "project_defines.h"

#include "driver/UART_MUX/uart_mux.h"
#include "driver/DEBUG/debug.h"
#include "driver/XKC-KL200/xkc_kl200.h"
#include "driver/BLD_300B/bld_300b.h"
#include "driver/NEXTION/nextion.h"


// ros sub callbacks
void SUB_Locomotion_app_cb(int direction, float velocity);
void SUB_MaxVelocity_app_cb(int MaxVelocity);
void SUB_Tower_app_cb(int method, int value);
void SUB_Alert_app_cb(int AlertCode);
void SUB_Load_app_cb(int LoadCode);
void SUB_Battery_app_cb(int BatteryLevel);

// ros pub callbacks
bool PUB_Sensor_app_cb(char *buffer, int *size);
bool PUB_AlertSauron_app_cb(char *buffer, int *size);

// MOTOR callbacks
void wheel_at_start_cb();
void wheel_at_stop_cb();
void ptz_at_start_cb();
void ptz_at_stop_cb();
void ROS_at_transport_disconnected_cb();
void ROS_at_transport_connected_cb();

// MOTOR alarm callbacks
void wheel_MotorFrontRight_at_fault();
void wheel_MotorFrontRight_at_NoFault();
void wheel_MotorFrontLeft_at_fault();
void wheel_MotorFrontLeft_at_NoFault();
void wheel_MotorBackRight_at_fault();
void wheel_MotorBackRight_at_NoFault();
void wheel_MotorBackLeft_at_fault();
void wheel_MotorBackLeft_at_NoFault();
void ptz_motor_at_fault();
void ptz_motor_at_NoFault();
