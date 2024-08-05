#include "Arduino.h"
#include "../../driver/DEBUG/debug.h"
#include "../../driver/BLD_300B/bld_300b.h"
#include "wheel.h"

#define WHEEL_FRONT_RIGHT_INIT(fault_cb, NoFault_cb) \
    {                                                \
        BLD300B_driver3_init(fault_cb, NoFault_cb);  \
    }
#define WHEEL_FRONT_LEFT_INIT(fault_cb, NoFault_cb) \
    {                                               \
        BLD300B_driver2_init(fault_cb, NoFault_cb); \
    }
#define WHEEL_BACK_RIGHT_INIT(fault_cb, NoFault_cb) \
    {                                               \
        BLD300B_driver5_init(fault_cb, NoFault_cb); \
    }
#define WHEEL_BACK_LEFT_INIT(fault_cb, NoFault_cb)  \
    {                                               \
        BLD300B_driver4_init(fault_cb, NoFault_cb); \
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

#define WHEEL_FRONT_LEFT_GET_SPEED() BLD300B_driver2_get_duty()

int _wheel_MaxSpeed = 0;
wheel_at_start_cb_t _wheel_at_start_cb = NULL;
wheel_at_stop_cb_t _wheel_at_stop_cb = NULL;

void wheel_init(wheel_at_start_cb_t wheel_at_start_cb, wheel_at_stop_cb_t wheel_at_stop_cb,
                wheel_at_motor_fault_cb_t FrontRight_fault_cb, wheel_at_motor_NoFault_cb_t FrontRight_NoFault_cb,
                wheel_at_motor_fault_cb_t FrontLeft_fault_cb, wheel_at_motor_NoFault_cb_t FrontLeft_NoFault_cb,
                wheel_at_motor_fault_cb_t BackRight_fault_cb, wheel_at_motor_NoFault_cb_t BackRight_NoFault_cb,
                wheel_at_motor_fault_cb_t BackLeft_fault_cb, wheel_at_motor_NoFault_cb_t BackLeft_NoFault_cb)
{
    // para iniciar a velocidad maxima cero
    _wheel_MaxSpeed = 0;

    // inicializar  ruedas
    WHEEL_FRONT_RIGHT_INIT(FrontRight_fault_cb, FrontRight_NoFault_cb);
    WHEEL_FRONT_LEFT_INIT(FrontLeft_fault_cb, FrontLeft_NoFault_cb);
    WHEEL_BACK_RIGHT_INIT(BackRight_fault_cb, BackRight_NoFault_cb);
    WHEEL_BACK_LEFT_INIT(BackLeft_fault_cb, BackLeft_NoFault_cb);

    // callbacks a nivel de modulo wheel
    _wheel_at_start_cb = wheel_at_start_cb;
    _wheel_at_stop_cb = wheel_at_stop_cb;
}
void wheel_stop()
{
    WHEEL_FRONT_RIGHT_STOP();
    WHEEL_FRONT_LEFT_STOP();
    WHEEL_BACK_RIGHT_STOP();
    WHEEL_BACK_LEFT_STOP();

    _wheel_at_stop_cb();
}
void wheel_move_forward(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));

    _wheel_at_start_cb();
}
void wheel_move_backward(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));

    _wheel_at_start_cb();
}
void wheel_turn_right(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));

    _wheel_at_start_cb();
}
void wheel_turn_left(float speed)
{
    WHEEL_FRONT_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_FRONT_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_RIGHT_MOVE_FORWARD((int)(speed * _wheel_MaxSpeed));
    WHEEL_BACK_LEFT_MOVE_BACKWARD((int)(speed * _wheel_MaxSpeed));

    _wheel_at_start_cb();
}
void wheel_set_MaxSpeed(int MaxSpeed) // 0-100%
{
    _wheel_MaxSpeed = MaxSpeed;
}

int wheel_get_MaxSpeed()
{
    return _wheel_MaxSpeed;
}

int wheel_FrontLeft_get_speed()
{
    return WHEEL_FRONT_LEFT_GET_SPEED();
}
