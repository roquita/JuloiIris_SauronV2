#include "Arduino.h"
#include "bld_300b.h"
#include "../DEBUG/debug.h"
#include "project_defines.h"

BLD300B_at_fault_cb_t _BLD300B_at_fault_5_cb = NULL;
BLD300B_at_NoFault_cb_t _BLD300B_at_NoFault_5_cb = NULL;
BLD300B_alarm_t _driver5_alarm_status = BLD300B_ALARM_NONE;
int _driver5_speed_duty = 0;
int _driver5_speed_time_up = 0;
int _driver5_speed_time_down = 0;

void driver5_alarm_cb()
{
    bool fault_happenned = (digitalRead(DRIVER_5_ALARM_PIN) == 0);
    fault_happenned ? _BLD300B_at_fault_5_cb() : _BLD300B_at_NoFault_5_cb();
}
void driver5_speed_cb()
{
    int now = millis();
    if (digitalRead(DRIVER_5_SPEED_PIN) == 1)
    {
        int high = _driver5_speed_time_up - _driver5_speed_time_down;
        int total = now - _driver5_speed_time_up;
        _driver5_speed_duty = 100 - (high * 100) / total; // complement

        _driver5_speed_time_up = now;
    }
    else
    {
        _driver5_speed_time_down = now;
    }
}
void BLD300B_driver5_init(BLD300B_at_fault_cb_t BLD300B_at_fault_cb, BLD300B_at_NoFault_cb_t BLD300B_at_NoFault_cb)
{
    // set callback
    _BLD300B_at_fault_5_cb = BLD300B_at_fault_cb;
    _BLD300B_at_NoFault_5_cb = BLD300B_at_NoFault_cb;

    // general
    analogWriteResolution(8);

    /***************************** DRIVER 5  ************************/
    // alarm
    pinMode(DRIVER_5_ALARM_PIN, INPUT);
    attachInterrupt(DRIVER_5_ALARM_PIN, driver5_alarm_cb, CHANGE);
    // speed
    pinMode(DRIVER_5_SPEED_PIN, INPUT);
    // attachInterrupt(DRIVER_5_SPEED_PIN, driver5_speed_cb, CHANGE);
    _driver5_speed_duty = 0;
    // direction
    pinMode(DRIVER_5_FR_PIN, OUTPUT);
    digitalWrite(DRIVER_5_FR_PIN, BLD300B_DIRECTION_FORWARD);
    // speed
    analogWriteFrequency(DRIVER_5_SV_PIN, 2000.0);
    analogWrite(DRIVER_5_SV_PIN, 255); // 0=100% - 255=0%
}
void BLD300B_driver5_set_duty(int duty) // 0-100%
{
    float fduty = (100 - duty) / 100.0; // complement
    int PwmVal = (int)(255 * fduty);
    analogWrite(DRIVER_5_SV_PIN, PwmVal);
}
void BLD300B_driver5_set_direction(BLD300B_direction_t direction)
{
    digitalWrite(DRIVER_5_FR_PIN, direction);
}
BLD300B_alarm_t BLD300B_driver5_get_alarm()
{
    noInterrupts();
    BLD300B_alarm_t _driver5_alarm_status_cpy = _driver5_alarm_status;
    _driver5_alarm_status = BLD300B_ALARM_NONE;
    interrupts();
    return _driver5_alarm_status_cpy;
}
int BLD300B_driver5_get_duty() // 0-100%
{
    return _driver5_speed_duty;
}