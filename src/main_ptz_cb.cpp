#include "main.h"


void ptz_at_start_cb()
{
    load_toogling_circulin();
}

void ptz_at_stop_cb()
{
    load_TurnOff_circulin();
}


void ptz_motor_at_fault()
{
    alarm_set_MotorPtz();
}
void ptz_motor_at_NoFault()
{
    alarm_clear_MotorPtz();
}
