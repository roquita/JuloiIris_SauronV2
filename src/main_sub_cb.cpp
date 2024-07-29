#include "main.h"

void SUB_Locomotion_app_cb(int direction, float velocity)
{
    switch (direction)
    {
    case 0: // PARAR
        wheel_stop();
        break;
    case 1: // ADELANTE
        wheel_move_forward(velocity);
        break;
    case 2: // ATRAS
        wheel_move_backward(velocity);
        break;
    case 3: // GIRAR DERECHA
        wheel_turn_right(velocity);
        break;
    default: // GIRAR IZQUIERDA
        wheel_turn_left(velocity);
        break;
    }
}
void SUB_MaxVelocity_app_cb(int MaxVelocity)
{
    wheel_set_MaxSpeed(MaxVelocity);
}
void SUB_Tower_app_cb(int method, int value)
{
    switch (method)
    {
    case 1: // BY PULSE
        value == 1 ? ptz_MoveUp_by_pulse() : ptz_MoveDown_by_pulse();
        break;
    case 2: // BY LIMIT
        value == 1 ? ptz_MoveUp_by_limit() : ptz_MoveDown_by_limit();
        break;
    default: // BY POSITION
        ptz_move_by_position(value);
        break;
    }
}
void SUB_Alert_app_cb(int AlertCode)
{
    switch (AlertCode)
    {
    case 1: // NO INTERNET
        wheel_stop();
        // ptz_stop();
        // load_Toogling_1s_5s_PilotRed();
        screen_queue_NoInternet_Notification();
        break;
    case 2: // SI INTERNET
        screen_goto_MainPage();
        screen_queue_YesInternet_Notificacion();
        break;
    default: // BATERIA BAJA
        screen_queue_LowBatery_Notificacion();
        break;
    }
}
void SUB_Load_app_cb(int LoadCode)
{
    switch (LoadCode)
    {
    case 0: //  APAGAR LUCES
         load_TurnOff_light();
        break;
    default: // ENCENDER LUCES
         load_TurnOn_light();
        break;
    }
}
void SUB_Battery_app_cb(int BatteryLevel)
{
    screen_print_BatteryLevel(BatteryLevel);
}
