#include "Arduino.h"
#include "bld_300b.h"
#include "../DEBUG/debug.h"
#include "project_defines.h"

BLD300B_alarm_t _driver1_alarm_status = BLD300B_ALARM_NONE;
int _driver1_alarm_pulses = 0;
int _driver1_alarm_time = 0;
int _driver1_speed_duty = 0;
int _driver1_speed_time_up = 0;
int _driver1_speed_time_down = 0;

void driver1_alarm_cb()
{
    int now = millis();
    if (digitalRead(DRIVER_1_ALARM_PIN) == 1)
    {
        int elapsed = now - _driver1_alarm_time;
        if (elapsed < 1100)
        {
            _driver1_alarm_pulses++;
        }
        else if (elapsed < 5100)
        {
            switch (_driver1_alarm_pulses)
            {
            case 2:
                _driver1_alarm_status = BLD300B_ALARM_OVERVOLTAGE;
                break;
            case 3:
                _driver1_alarm_status = BLD300B_ALARM_POWER_TUBE_OVERCURRENT;
                break;
            case 4:
                _driver1_alarm_status = BLD300B_ALARM_OVERCURRENT;
                break;
            case 5:
                _driver1_alarm_status = BLD300B_ALARM_UNDERVOLTAGE;
                break;
            case 6:
                _driver1_alarm_status = BLD300B_ALARM_HALL;
                break;
            case 7:
                _driver1_alarm_status = BLD300B_ALARM_LOCKED;
                break;
            case 8:
                _driver1_alarm_status = BLD300B_ALARM_TWO_OR_MORE;
                break;
            default:
                _driver1_alarm_status = BLD300B_ALARM_UNKNOWN;
                break;
            }
            _driver1_alarm_pulses = 0;
        }
    }
    else
    {
        _driver1_alarm_time = now;
    }
}
void driver1_speed_cb()
{
    int now = millis();
    if (digitalRead(DRIVER_1_ALARM_PIN) == 1)
    {
        int high = _driver1_speed_time_up - _driver1_speed_time_down;
        int total = now - _driver1_speed_time_up;
        _driver1_speed_duty = 100 - (high * 100) / total; // complement

        _driver1_speed_time_up = now;
    }
    else
    {
        _driver1_speed_time_down = now;
    }
}
void BLD300B_driver1_init()
{
    // general
    analogWriteResolution(8);

    /***************************** DRIVER 5  ************************/
    // alarm
    pinMode(DRIVER_1_ALARM_PIN, INPUT);
    //attachInterrupt(DRIVER_1_ALARM_PIN, driver5_alarm_cb, CHANGE);
    _driver1_alarm_pulses = 0;
    _driver1_alarm_time = millis();
    // speed
    pinMode(DRIVER_1_SPEED_PIN, INPUT);
    //attachInterrupt(DRIVER_1_SPEED_PIN, driver5_speed_cb, CHANGE);
    _driver1_speed_duty = 0;
    // direction
    pinMode(DRIVER_1_FR_PIN, OUTPUT);
    digitalWrite(DRIVER_1_FR_PIN, BLD300B_DIRECTION_FORWARD);
    // speed
    analogWriteFrequency(DRIVER_1_SV_PIN, 2000.0);
    analogWrite(DRIVER_1_SV_PIN, 255); // 0=100% - 255=0%
}
void BLD300B_driver1_set_duty(int duty) // 0-100%
{
    float fduty = (100 - duty) / 100.0; // complement
    int PwmVal = (int)(255 * fduty);
    analogWrite(DRIVER_1_SV_PIN, PwmVal);
}
void BLD300B_driver1_set_direction(BLD300B_direction_t direction)
{
    digitalWrite(DRIVER_1_FR_PIN, direction);
}
BLD300B_alarm_t BLD300B_driver1_get_alarm()
{
    noInterrupts();
    BLD300B_alarm_t _driver1_alarm_status_cpy = _driver1_alarm_status;
    _driver1_alarm_status = BLD300B_ALARM_NONE;
    interrupts();
    return _driver1_alarm_status_cpy;
}
int BLD300B_driver1_get_duty() // 0-100%
{
    return _driver1_speed_duty;
}