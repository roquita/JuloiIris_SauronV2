#pragma once

typedef void (*wheel_at_start_cb_t)(void);
typedef void (*wheel_at_stop_cb_t)(void);
typedef void (*wheel_at_motor_fault_cb_t)(void);
typedef void (*wheel_at_motor_NoFault_cb_t)(void);

void wheel_init(wheel_at_start_cb_t, wheel_at_stop_cb_t,
                wheel_at_motor_fault_cb_t FrontRight_fault_cb, wheel_at_motor_NoFault_cb_t FrontRight_NoFault_cb,
                wheel_at_motor_fault_cb_t FrontLeft_fault_cb, wheel_at_motor_NoFault_cb_t FrontLeft_NoFault_cb,
                wheel_at_motor_fault_cb_t BackRight_fault_cb, wheel_at_motor_NoFault_cb_t BackRight_NoFault_cb,
                wheel_at_motor_fault_cb_t BackLeft_fault_cb, wheel_at_motor_NoFault_cb_t BackLeft_NoFault_cb);
void wheel_stop();
void wheel_move_forward(float speed);
void wheel_move_backward(float speed);
void wheel_turn_right(float speed);
void wheel_turn_left(float speed);
void wheel_set_MaxSpeed(int MaxSpeed); // 0-100%
int wheel_get_MaxSpeed();
int wheel_FrontLeft_get_speed();
