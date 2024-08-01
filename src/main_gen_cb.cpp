#include "main.h"

void wheel_at_start_cb()
{
    load_toogling_circulin();
}

void wheel_at_stop_cb()
{
    load_TurnOff_circulin();
}

void ptz_at_start_cb()
{
    load_toogling_circulin();
}

void ptz_at_stop_cb()
{
    load_TurnOff_circulin();
}

void ROS_at_transport_disconnected_cb()
{
    load_toogling_PilotYellow();
    wheel_stop();
}

void ROS_at_transport_connected_cb()
{
    load_TurnOff_PilotYellow();
}