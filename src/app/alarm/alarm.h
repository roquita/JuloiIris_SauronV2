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


int alarm_get_MotorFrontRight();
int alarm_get_MotorFrontLeft();
int alarm_get_MotorBackRight();
int alarm_get_MotorBackLeft();
int alarm_get_MotorPtz();

