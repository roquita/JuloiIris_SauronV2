#pragma once

void ptz_init();
void ptz_MoveUp_by_pulse();
void ptz_MoveDown_by_pulse();
void ptz_MoveUp_by_limit();
void ptz_MoveDown_by_limit();
void ptz_move_by_position(int position); // 0-100%
void ptz_stop();
int ptz_get_position();
bool ptz_uls_pressed();
bool ptz_lls_pressed();
