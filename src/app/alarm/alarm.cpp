#include "alarm.h"
#include "Arduino.h"

bool _MotorFrontRight_alarm_triggered = false;
bool _MotorFrontLeft_alarm_triggered = false;
bool _MotorBackRight_alarm_triggered = false;
bool _MotorBackLeft_alarm_triggered = false;
bool _MotorPtz_alarm_triggered = false;

void alarm_init() {}

void alarm_set_MotorFrontRight() { _MotorFrontRight_alarm_triggered = true; }
void alarm_set_MotorFrontLeft() { _MotorFrontLeft_alarm_triggered = true; }
void alarm_set_MotorBackRight() { _MotorBackRight_alarm_triggered = true; }
void alarm_set_MotorBackLeft() { _MotorBackLeft_alarm_triggered = true; }
void alarm_set_MotorPtz() { _MotorPtz_alarm_triggered = true; }

void alarm_clear_MotorFrontRight() { _MotorFrontRight_alarm_triggered = false; }
void alarm_clear_MotorFrontLeft() { _MotorFrontLeft_alarm_triggered = false; }
void alarm_clear_MotorBackRight() { _MotorBackRight_alarm_triggered = false; }
void alarm_clear_MotorBackLeft() { _MotorBackLeft_alarm_triggered = false; }
void alarm_clear_MotorPtz() { _MotorPtz_alarm_triggered = false; }

bool alarm_get_MotorFrontRight() { return _MotorFrontRight_alarm_triggered; }
bool alarm_get_MotorFrontLeft() { return _MotorFrontLeft_alarm_triggered; }
bool alarm_get_MotorBackRight() { return _MotorBackRight_alarm_triggered; }
bool alarm_get_MotorBackLeft() { return _MotorBackLeft_alarm_triggered; }
bool alarm_get_MotorPtz() { return _MotorPtz_alarm_triggered; }