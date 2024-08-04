#pragma once

void alarm_init();

void alarm_set_MotorFrontRight();
void alarm_set_MotorFrontLeft();
void alarm_set_MotorBackRight();
void alarm_set_MotorBackLeft();
void alarm_set_MotorPtz();


void alarm_clear_MotorFrontRight();
void alarm_clear_MotorFrontLeft();
void alarm_clear_MotorBackRight();
void alarm_clear_MotorBackLeft();
void alarm_clear_MotorPtz();


bool alarm_get_MotorFrontRight();
bool alarm_get_MotorFrontLeft();
bool alarm_get_MotorBackRight();
bool alarm_get_MotorBackLeft();
bool alarm_get_MotorPtz();

