#include "Arduino.h"
#include "../../driver/XKC-KL200/xkc_kl200.h"
#include "../../driver/DEBUG/debug.h"

static int Front_mm = 0;
static int RightFront_mm = 0;
static int RightMiddle_mm = 0;
static int RightBack_mm = 0;
static int LeftFront_mm = 0;
static int LeftMiddle_mm = 0;
static int LeftBack_mm = 0;
static int BackRight_mm = 0;
static int BackLeft_mm = 0;

static int _state_machine_counter = 0;
static int _sensor_target = 0;
static int _timeout = 0;

void distance_init()
{
    xkc_kl200_init();
}
void distance_loop()
{
    switch (_state_machine_counter)
    {
    case 100:
    {
        bool timeout_triggered = false;
        bool data_readed = false;

        if (_timeout >= 2)
        {
            timeout_triggered = true;
        }

        if (xkc_kl200_available() == true)
        {
            int data = 0;
            bool success = xkc_kl200_read(&data) == true;
            if (success)
            {
                data_readed = true;
                switch (_sensor_target)
                {
                case 6:
                    Front_mm = data;
                    break;
                case 9:
                    RightFront_mm = data;
                    break;
                case 8:
                    RightMiddle_mm = data;
                    break;
                case 7:
                    RightBack_mm = data;
                    break;
                case 1:
                    LeftFront_mm = data;
                    break;
                case 2:
                    LeftMiddle_mm = data;
                    break;
                case 3:
                    LeftBack_mm = data;
                    break;
                case 4:
                    BackRight_mm = data;
                    break;
                default:
                    BackLeft_mm = data;
                    break;
                }
            }
        }

        _timeout++;

        if (timeout_triggered || data_readed)
        {
            _timeout = 0;

            switch (_sensor_target)
            {
            case 6:
                _state_machine_counter = 1;
                break;
            case 9:
                _state_machine_counter = 2;
                break;
            case 8:
                _state_machine_counter = 3;
                break;
            case 7:
                _state_machine_counter = 4;
                break;
            case 1:
                _state_machine_counter = 5;
                break;
            case 2:
                _state_machine_counter = 6;
                break;
            case 3:
                _state_machine_counter = 7;
                break;
            case 4:
                _state_machine_counter = 8;
                break;
            default:
                _state_machine_counter = 0;
                break;
            }
        }

        break;
    }
    case 0:
        xkc_kl200_select(XKC_KL200_DEVICE_6);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 6;
        break;
    case 1:
        xkc_kl200_select(XKC_KL200_DEVICE_9);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 9;
        break;
    case 2:
        xkc_kl200_select(XKC_KL200_DEVICE_8);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 8;
        break;
    case 3:
        xkc_kl200_select(XKC_KL200_DEVICE_7);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 7;
        break;
    case 4:
        xkc_kl200_select(XKC_KL200_DEVICE_1);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 1;
        break;
    case 5:
        xkc_kl200_select(XKC_KL200_DEVICE_2);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 2;
        break;
    case 6:
        xkc_kl200_select(XKC_KL200_DEVICE_3);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 3;
        break;
    case 7:
        xkc_kl200_select(XKC_KL200_DEVICE_4);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 4;
        break;
    case 8:
        xkc_kl200_select(XKC_KL200_DEVICE_5);
        xkc_kl200_request_data();
        _state_machine_counter = 100;
        _sensor_target = 5;
        break;
    default:
        _timeout = 0;
        _state_machine_counter = 0;
        _sensor_target = 0;
        break;
    }
}
int distance_get_Front_mm()
{
    return Front_mm;
}
int distance_get_RightFront_mm()
{
    return RightFront_mm;
}
int distance_get_RightMiddle_mm()
{
    return RightMiddle_mm;
}
int distance_get_RightBack_mm()
{
    return RightBack_mm;
}
int distance_get_LeftFront_mm()
{
    return LeftFront_mm;
}
int distance_get_LeftMiddle_mm()
{
    return LeftMiddle_mm;
}
int distance_get_LeftBack_mm()
{
    return LeftBack_mm;
}
int distance_get_BackRight_mm()
{
    return BackRight_mm;
}
int distance_get_BackLeft_mm()
{
    return BackLeft_mm;
}