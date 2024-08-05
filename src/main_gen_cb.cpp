#include "main.h"

void ROS_at_transport_disconnected_cb()
{
    load_toogling_PilotYellow();
    wheel_stop();
}

void ROS_at_transport_connected_cb()
{
    load_TurnOff_PilotYellow();
}