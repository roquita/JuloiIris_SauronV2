#include "Arduino.h"
#include "bld_300b.h"
#include "../DEBUG/debug.h"
#include "project_defines.h"

BLD300B_at_fault_cb_t _BLD300B_at_fault_4_cb = NULL;
BLD300B_at_NoFault_cb_t _BLD300B_at_NoFault_4_cb = NULL;
BLD300B_alarm_t _driver4_alarm_status = BLD300B_ALARM_NONE;
int _driver4_speed_duty = 0;
int _driver4_speed_time_up = 0;
int _driver4_speed_time_down = 0;

void driver4_alarm_cb()
{
    bool fault_happenned = (digitalRead(DRIVER_4_ALARM_PIN) == 0);
    fault_happenned ? _BLD300B_at_fault_4_cb() : _BLD300B_at_NoFault_4_cb();
}
void driver4_speed_cb()
{
    int now = millis();
    if (digitalRead(DRIVER_4_ALARM_PIN) == 1)
    {
        int high = _driver4_speed_time_up - _driver4_speed_time_down;
        int total = now - _driver4_speed_time_up;
        _driver4_speed_duty = 100 - (high * 100) / total; // complement

        _driver4_speed_time_up = now;
    }
    else
    {
        _driver4_speed_time_down = now;
    }
}
void BLD300B_driver4_init(BLD300B_at_fault_cb_t BLD300B_at_fault_cb, BLD300B_at_NoFault_cb_t BLD300B_at_NoFault_cb)
{
    // set callback
    _BLD300B_at_fault_4_cb = BLD300B_at_fault_cb;
    _BLD300B_at_NoFault_4_cb = BLD300B_at_NoFault_cb;

    // general
    analogWriteResolution(8);

    /***************************** DRIVER 5  ************************/
    // alarm
    pinMode(DRIVER_4_ALARM_PIN, INPUT);
    attachInterrupt(DRIVER_4_ALARM_PIN, driver4_alarm_cb, CHANGE);
    // speed
    pinMode(DRIVER_4_SPEED_PIN, INPUT);
    // attachInterrupt(DRIVER_4_SPEED_PIN, driver5_speed_cb, CHANGE);
    _driver4_speed_duty = 0;
    // direction
    pinMode(DRIVER_4_FR_PIN, OUTPUT);
    digitalWrite(DRIVER_4_FR_PIN, BLD300B_DIRECTION_FORWARD);
    // speed
    analogWriteFrequency(DRIVER_4_SV_PIN, 2000.0);
    analogWrite(DRIVER_4_SV_PIN, 255); // 0=100% - 255=0%
}
void BLD300B_driver4_set_duty(int duty) // 0-100%
{
    float fduty = (100 - duty) / 100.0; // complement
    int PwmVal = (int)(255 * fduty);
    analogWrite(DRIVER_4_SV_PIN, PwmVal);
}
void BLD300B_driver4_set_direction(BLD300B_direction_t direction)
{
    digitalWrite(DRIVER_4_FR_PIN, direction);
}
BLD300B_alarm_t BLD300B_driver4_get_alarm()
{
    noInterrupts();
    BLD300B_alarm_t _driver4_alarm_status_cpy = _driver4_alarm_status;
    _driver4_alarm_status = BLD300B_ALARM_NONE;
    interrupts();
    return _driver4_alarm_status_cpy;
}
int BLD300B_driver4_get_duty() // 0-100%
{
    return _driver4_speed_duty;
}