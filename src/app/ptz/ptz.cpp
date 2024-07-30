#include "Arduino.h"
#include "ptz.h"
#include "../../project_defines.h"
#include "../../driver/DEBUG/debug.h"
#include "../../driver/BLD_300B/bld_300b.h"

typedef enum
{
    PTZ_DIR_MOVING_UP,
    PTZ_DIR_MOVING_DOWN,
    PTZ_DIR_STOPPED,
} ptz_direction_t;

ptz_at_start_cb_t _ptz_at_start_cb = NULL;
ptz_at_stop_cb_t _ptz_at_stop_cb = NULL;

#define PTZ_INIT()              \
    {                           \
        BLD300B_driver1_init(); \
    }
#define PTZ_MOVE_UP()                                              \
    {                                                              \
        BLD300B_driver1_set_direction(BLD300B_DIRECTION_REVERSED); \
        BLD300B_driver1_set_duty(50);                              \
    }
#define PTZ_MOVE_DOWN()                                           \
    {                                                             \
        BLD300B_driver1_set_direction(BLD300B_DIRECTION_FORWARD); \
        BLD300B_driver1_set_duty(50);                             \
    }
#define PTZ_STOP()                   \
    {                                \
        BLD300B_driver1_set_duty(0); \
    }

#define UPPER_LIMIT_SWITCH_INIT(callback)                 \
    {                                                     \
        pinMode(SWITCH_1_PIN, INPUT);                     \
        attachInterrupt(SWITCH_1_PIN, callback, FALLING); \
    }
#define UPPER_LIMIT_SWITCH_IS_TRIGGERED() (digitalRead(SWITCH_1_PIN) == 0)
#define LOWER_LIMIT_SWITCH_INIT(callback)                 \
    {                                                     \
        pinMode(SWITCH_2_PIN, INPUT);                     \
        attachInterrupt(SWITCH_2_PIN, callback, FALLING); \
    }
#define LOWER_LIMIT_SWITCH_IS_TRIGGERED() (digitalRead(SWITCH_2_PIN) == 0)

ptz_direction_t _ptz_direction = PTZ_DIR_STOPPED;
int _ptz_position = 0;     // 0-100%
int _target_position = -1; // -1: no_tracking  ,  [0-100]:tracking
static IntervalTimer _timer_position;
static IntervalTimer _timer_pulse;

void timer_pulse_cb()
{
    ptz_stop();
}
void timer_position_cb()
{
    if (_ptz_direction == PTZ_DIR_MOVING_UP)
    {
        _ptz_position = ((_ptz_position >= 100) ? 100 : _ptz_position + 1);
        if ((_target_position != -1) && (_ptz_position >= _target_position))
        {
            ptz_stop();
        }
    }
    else if (_ptz_direction == PTZ_DIR_MOVING_DOWN)
    {
        _ptz_position = ((_ptz_position <= 0) ? 0 : _ptz_position - 1);
        if ((_target_position != -1) && (_ptz_position <= _target_position))
        {
            ptz_stop();
        }
    }
}
void upper_LimitSwitch_cb()
{
    ptz_stop();

    _ptz_position = 100;
}
void lower_LimitSwitch_cb()
{
    ptz_stop();

    _ptz_position = 0;
}

void ptz_init(ptz_at_start_cb_t ptz_at_start_cb, ptz_at_stop_cb_t ptz_at_stop_cb)
{
    PTZ_INIT();
    UPPER_LIMIT_SWITCH_INIT(upper_LimitSwitch_cb);
    LOWER_LIMIT_SWITCH_INIT(lower_LimitSwitch_cb);

    // TIMER POSITION 100ms
    _timer_position.begin(timer_position_cb, 100 * 1000UL);

    _ptz_at_start_cb = ptz_at_start_cb;
    _ptz_at_stop_cb = ptz_at_stop_cb;

    ptz_stop();

    DEBUG_PRINTLN(F("PTZ INIT DONE"));
}
void ptz_MoveUp_by_pulse()
{
    _target_position = -1;
    _timer_pulse.end();

    if (UPPER_LIMIT_SWITCH_IS_TRIGGERED())
    {
        DEBUG_PRINTLN(F("upper_LimitSwitch_triggered"));
        return;
    }

    PTZ_MOVE_UP();
    _ptz_direction = PTZ_DIR_MOVING_UP;
    _timer_pulse.begin(timer_pulse_cb, 2000 * 1000UL);

    _ptz_at_start_cb();
}
void ptz_MoveDown_by_pulse()
{
    _target_position = -1;
    _timer_pulse.end();

    if (LOWER_LIMIT_SWITCH_IS_TRIGGERED())
    {
        DEBUG_PRINTLN(F("lower_LimitSwitch_triggered"));
        return;
    }

    PTZ_MOVE_DOWN();
    _ptz_direction = PTZ_DIR_MOVING_DOWN;
    _timer_pulse.begin(timer_pulse_cb, 2000 * 1000UL);

    _ptz_at_start_cb();
}
void ptz_MoveUp_by_limit()
{
    _target_position = -1;
    _timer_pulse.end();

    if (UPPER_LIMIT_SWITCH_IS_TRIGGERED())
    {
        DEBUG_PRINTLN(F("upper_LimitSwitch_triggered"));
        return;
    }

    PTZ_MOVE_UP();
    _ptz_direction = PTZ_DIR_MOVING_UP;

    _ptz_at_start_cb();
}
void ptz_MoveDown_by_limit()
{
    _target_position = -1;
    _timer_pulse.end();

    if (LOWER_LIMIT_SWITCH_IS_TRIGGERED())
    {
        DEBUG_PRINTLN(F("lower_LimitSwitch_triggered"));
        return;
    }

    PTZ_MOVE_DOWN();
    _ptz_direction = PTZ_DIR_MOVING_DOWN;

    _ptz_at_start_cb();
}
void ptz_move_by_position(int target_position) // 0-100%
{
    _target_position = -1;
    _timer_pulse.end();

    if (target_position >= _ptz_position)
    {
        if (UPPER_LIMIT_SWITCH_IS_TRIGGERED())
        {
            DEBUG_PRINTLN(F("upper_LimitSwitch_triggered"));
            return;
        }
        PTZ_MOVE_UP();
        _ptz_direction = PTZ_DIR_MOVING_UP;
        _target_position = target_position;

        _ptz_at_start_cb();
    }
    else
    {
        if (LOWER_LIMIT_SWITCH_IS_TRIGGERED())
        {
            DEBUG_PRINTLN(F("lower_LimitSwitch_triggered"));
            return;
        }

        PTZ_MOVE_DOWN();
        _ptz_direction = PTZ_DIR_MOVING_DOWN;
        _target_position = target_position;

        _ptz_at_start_cb();
    }
}
void ptz_stop()
{
    PTZ_STOP();
    _ptz_direction = PTZ_DIR_STOPPED;
    _timer_pulse.end();
    _target_position = -1;

    _ptz_at_stop_cb();
}
int ptz_get_position()
{
    return _ptz_position;
}

bool ptz_uls_pressed()
{
    return UPPER_LIMIT_SWITCH_IS_TRIGGERED();
}
bool ptz_lls_pressed()
{
    return LOWER_LIMIT_SWITCH_IS_TRIGGERED();
}