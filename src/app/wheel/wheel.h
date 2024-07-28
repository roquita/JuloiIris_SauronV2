#pragma once 



void wheel_init();
void wheel_stop();
void wheel_move_forward(float speed);
void wheel_move_backward(float speed);
void wheel_turn_right(float speed);
void wheel_turn_left(float speed);
void wheel_set_MaxSpeed(int MaxSpeed); // 0-100% 
int wheel_get_MaxSpeed();



