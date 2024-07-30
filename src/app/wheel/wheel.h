#pragma once

typedef void (*wheel_at_start_cb_t)(void);
typedef void (*wheel_at_stop_cb_t)(void);

void wheel_init(wheel_at_start_cb_t, wheel_at_stop_cb_t);
void wheel_stop();
void wheel_move_forward(float speed);
void wheel_move_backward(float speed);
void wheel_turn_right(float speed);
void wheel_turn_left(float speed);
void wheel_set_MaxSpeed(int MaxSpeed); // 0-100%
int wheel_get_MaxSpeed();
int wheel_FrontLeft_get_speed();
