#pragma once

typedef void (*ptz_at_start_cb_t)(void);
typedef void (*ptz_at_stop_cb_t)(void);

void ptz_init(ptz_at_start_cb_t, ptz_at_stop_cb_t);
void ptz_MoveUp_by_pulse();
void ptz_MoveDown_by_pulse();
void ptz_MoveUp_by_limit();
void ptz_MoveDown_by_limit();
void ptz_move_by_position(int position); // 0-100%
void ptz_stop();
int ptz_get_position();
bool ptz_uls_pressed();
bool ptz_lls_pressed();
