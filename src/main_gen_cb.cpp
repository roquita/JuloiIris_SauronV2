#include "main.h"

void MOTOR_at_start_cb()
{
    load_toogling_circulin();
}

void MOTOR_at_stop_cb()
{
    load_TurnOff_circulin();
}

void ROS_at_transport_disconnected_cb()
{
    load_toogling_PilotYellow();
}

void ROS_at_transport_connected_cb()
{
    load_TurnOff_PilotYellow();
}