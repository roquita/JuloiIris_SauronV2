#include "Arduino.h"
#include "../../driver/DEBUG/debug.h"
#include "../../driver/BLD_300B/bld_300b.h"
#include "wheel.h"

#define WHEEL_FRONT_RIGHT_INIT() \
    {                            \
        BLD300B_driver3_init();  \
    }
#define WHEEL_FRONT_LEFT_INIT() \
    {                           \
        BLD300B_driver2_init(); \
    }
#define WHEEL_BACK_RIGHT_INIT() \
    {                           \
        BLD300B_driver5_init(); \
    }
#define WHEEL_BACK_LEFT_INIT()  \
    {                           \
        BLD300B_driver4_init(); \
    }

#define WHEEL_FRONT_RIGHT_MOVE_FORWARD(x)                         \
    {                                                             \
        BLD300B_driver3_set_direction(BLD300B_DIRECTION_FORWARD); \
        BLD300B_driver3_set_duty(x);                              \
    }
#define WHEEL_FRONT_LEFT_MOVE_FORWARD(x)                           \
    {                                                              \
        BLD300B_driver2_set_direction(BLD300B_DIRECTION_REVERSED); \
        BLD300B_driver2_set_duty(x);                               \
    }
#define WHEEL_BACK_RIGHT_MOVE_FORWARD(x)                          \
    {                                                             \
        BLD300B_driver5_set_direction(BLD300B_DIRECTION_FORWARD); \
        BLD300B_driver5_set_duty(x);                              \
    }
#define WHEEL_BACK_LEFT_MOVE_FORWARD(x)                            \
    {                                                              \
        BLD300B_driver4_set_direction(BLD300B_DIRECTION_REVERSED); \
        BLD300B_driver4_set_duty(x);                               \
    }

#define WHEEL_FRONT_RIGHT_MOVE_BACKWARD(x)                         \
    {                                                              \
        BLD300B_driver3_set_direction(BLD300B_DIRECTION_REVERSED); \
        BLD300B_driver3_set_duty(x);                               \
    }
#define WHEEL_FRONT_LEFT_MOVE_BACKWARD(x)                         \
    {                                                             \
        BLD300B_driver2_set_direction(BLD300B_DIRECTION_FORWARD); \
        BLD300B_driver2_set_duty(x);                              \
    }
#define WHEEL_BACK_RIGHT_MOVE_BACKWARD(x)                          \
    {                                                              \
        BLD300B_driver5_set_direction(BLD300B_DIRECTION_REVERSED); \
        BLD300B_driver5_set_duty(x);                               \
    }
#define WHEEL_BACK_LEFT_MOVE_BACKWARD(x)                          \
    {                                                             \
        BLD300B_driver4_set_direction(BLD300B_DIRECTION_FORWARD); \
        BLD300B_driver4_set_duty(x);                              \
    }

#define WHEEL_FRONT_RIGHT_STOP() BLD300B_driver3_set_duty(0)
#define WHEEL_FRONT_LEFT_STOP() BLD300B_driver2_set_duty(0)
#define WHEEL_BACK_RIGHT_STOP() BLD300B_driver5_set_duty(0)
#define WHEEL_BACK_LEFT_STOP() BLD300B_driver4_set_duty(0)

int _wheel_MaxSpeed = 0;

void wheel_init()
{
    _wheel_MaxSpeed = 0;
    WHEEL_FRONT_RIGHT_INIT();
    WHEEL_FRONT_LEFT_INIT();
    WHEEL_BACK_RIGHT_INIT();
    WHEEL_BACK_LEFT_INIT();
}
void wheel_stop()
{
    _wheel_MaxSpeed = 0;
    WHEEL_FRONT_RIGHT_STOP();
    WHEEL_FRONT_LEFT_STOP();
    WHEEL_BACK_RIGHT_STOP();
    WHEEL_BACK_LEFT_STOP();
}
void wheel_move_forward(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
}
void wheel_move_backward(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
}
void wheel_turn_right(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
}
void wheel_turn_left(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
}
void wheel_set_MaxSpeed(int MaxSpeed) // 0-100%
{
    _wheel_MaxSpeed = MaxSpeed;
}

int wheel_get_MaxSpeed()
{
    return _wheel_MaxSpeed;
}