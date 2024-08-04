#pragma once

typedef enum
{
    BLD300B_DIRECTION_REVERSED = 0,
    BLD300B_DIRECTION_FORWARD = 1,
} BLD300B_direction_t;
typedef enum
{
    BLD300B_ALARM_NONE,
    BLD300B_ALARM_OVERVOLTAGE,
    BLD300B_ALARM_POWER_TUBE_OVERCURRENT,
    BLD300B_ALARM_OVERCURRENT,
    BLD300B_ALARM_UNDERVOLTAGE,
    BLD300B_ALARM_HALL,
    BLD300B_ALARM_LOCKED,
    BLD300B_ALARM_TWO_OR_MORE,
    BLD300B_ALARM_UNKNOWN,
} BLD300B_alarm_t;

typedef void (*BLD300B_at_fault_cb_t)(void);
typedef void (*BLD300B_at_NoFault_cb_t)(void);

// break and enabled connected to COM

void BLD300B_driver1_init(BLD300B_at_fault_cb_t, BLD300B_at_NoFault_cb_t);
void BLD300B_driver1_set_duty(int duty); // 0-100%
void BLD300B_driver1_set_direction(BLD300B_direction_t direction);
BLD300B_alarm_t BLD300B_driver1_get_alarm();
int BLD300B_driver1_get_duty(); // 0-100%

void BLD300B_driver2_init(BLD300B_at_fault_cb_t, BLD300B_at_NoFault_cb_t);
void BLD300B_driver2_set_duty(int duty); // 0-100%
void BLD300B_driver2_set_direction(BLD300B_direction_t direction);
BLD300B_alarm_t BLD300B_driver2_get_alarm();
int BLD300B_driver2_get_duty(); // 0-100%

void BLD300B_driver3_init(BLD300B_at_fault_cb_t, BLD300B_at_NoFault_cb_t);
void BLD300B_driver3_set_duty(int duty); // 0-100%
void BLD300B_driver3_set_direction(BLD300B_direction_t direction);
BLD300B_alarm_t BLD300B_driver3_get_alarm();
int BLD300B_driver3_get_duty(); // 0-100%

void BLD300B_driver4_init(BLD300B_at_fault_cb_t, BLD300B_at_NoFault_cb_t);
void BLD300B_driver4_set_duty(int duty); // 0-100%
void BLD300B_driver4_set_direction(BLD300B_direction_t direction);
BLD300B_alarm_t BLD300B_driver4_get_alarm();
int BLD300B_driver4_get_duty(); // 0-100%

void BLD300B_driver5_init(BLD300B_at_fault_cb_t, BLD300B_at_NoFault_cb_t);
void BLD300B_driver5_set_duty(int duty); // 0-100%
void BLD300B_driver5_set_direction(BLD300B_direction_t direction);
BLD300B_alarm_t BLD300B_driver5_get_alarm();
int BLD300B_driver5_get_duty(); // 0-100%
