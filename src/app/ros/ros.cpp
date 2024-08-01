#include "Arduino.h"
#include "ros.h"
#include "../../driver/DEBUG/debug.h"
#include "../load/load.h"

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include "std_msgs/msg/string.h"
#include "std_msgs/msg/header.h"
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/u_int32.h>
#include <geometry_msgs/msg/twist.h>

// ros callbacks
Ros_at_transport_disconnected_t _Ros_at_transport_disconnected = NULL;
Ros_at_transport_conencted_t _Ros_at_transport_conencted = NULL;

// general
rcl_allocator_t _allocator;
rclc_support_t _support;
rcl_node_t _node;

// Subscriptor MsgToSauron
rcl_subscription_t _sMsgToSauron;
geometry_msgs__msg__Twist _mMsgToSauron;
rclc_executor_t _eMsgToSauron;
Locomotion_app_cb_t _Locomotion_app_cb = NULL;
MaxVelocity_app_cb_t _MaxVelocity_app_cb = NULL;
Tower_app_cb_t _Tower_app_cb = NULL;
Alert_app_cb_t _Alert_app_cb = NULL;
Load_app_cb_t _Load_app_cb = NULL;
Battery_app_cb_t _Battery_app_cb = NULL;

// Publisher Sensors
rcl_publisher_t _pSensor;
std_msgs__msg__String _mSensor;
rclc_executor_t _eSensor;
rcl_timer_t _tSensor;
Sensor_app_cb_t _Sensor_app_cb = NULL;

// Publisher Alert Sauron
rcl_publisher_t _pAlertSauron;
std_msgs__msg__String _mAlertSauron;
rclc_executor_t _eAlertSauron;
rcl_timer_t _tAlertSauron;
AlertSauron_app_cb_t _AlertSauron_app_cb = NULL;

void SUB_MsgToSauron_cb(const void *msgin)
{

    const geometry_msgs__msg__Twist *mMsgToSauron = (const geometry_msgs__msg__Twist *)msgin;

    int type = (int)mMsgToSauron->linear.z;
    float param1 = (float)mMsgToSauron->linear.x;
    float param2 = (float)mMsgToSauron->linear.y;

    if (type == 0)
    {
        int direction = (int)param1;
        float velocity = param2;
        DEBUG_PRINT(F("LOCOMOTION:\t"));
        DEBUG_PRINT(F("Direccion:"));
        DEBUG_PRINT(direction);
        DEBUG_PRINT(F(" || Rapidez:"));
        DEBUG_PRINTLN(velocity);

        bool valid_direction = (direction >= 0 && direction <= 4);
        bool valid_velocity = (velocity >= 0.0 && velocity <= 1.0);
        bool valid_params = valid_direction && valid_velocity;
        if (!valid_params)
        {
            DEBUG_PRINTLN(F("LOCOMOTION invalid params"));
            return;
        }

        // user callback
        if (_Locomotion_app_cb)
            _Locomotion_app_cb(direction, velocity);
    }
    else if (type == 1)
    {
        int MaxVelocity = (int)param1;
        DEBUG_PRINT(F("MAX VELOCITY(%):\t"));
        DEBUG_PRINTLN(MaxVelocity);

        bool valid_MaxVelocity = (MaxVelocity >= 0 && MaxVelocity <= 100);
        bool valid_params = valid_MaxVelocity;
        if (!valid_params)
        {
            DEBUG_PRINTLN(F("MAX VELOCITY invalid params"));
            return;
        }

        // user callback
        if (_MaxVelocity_app_cb)
            _MaxVelocity_app_cb(MaxVelocity);
    }
    else if (type == 2)
    {
        int method = (int)param1;
        int value = (int)param2;
        DEBUG_PRINT(F("TOWER:\t"));
        DEBUG_PRINT(F("Metodo:"));
        DEBUG_PRINT(method);
        DEBUG_PRINT(F(" || Valor:"));
        DEBUG_PRINTLN(value);

        bool valid_method_by_pulse = (method == 1 && (value == 1 || value == -1));
        bool valid_method_by_limit = (method == 2 && (value == 1 || value == -1));
        bool valid_mehotd_by_pos = (method == 3 && (value >= 0 && value <= 100));
        bool valid_params = valid_method_by_pulse || valid_method_by_limit || valid_mehotd_by_pos;
        if (!valid_params)
        {
            DEBUG_PRINTLN(F("TOWER invalid params"));
            return;
        }

        // user callback
        if (_Tower_app_cb)
            _Tower_app_cb(method, value);
    }
    else if (type == 3)
    {
        int AlertCode = (int)param1;
        DEBUG_PRINT(F("ALERT:\t"));
        DEBUG_PRINT(F("code:"));
        DEBUG_PRINTLN(AlertCode);

        bool valid_AlertCode = (AlertCode >= 1 && AlertCode <= 3);
        bool valid_params = valid_AlertCode;
        if (!valid_params)
        {
            DEBUG_PRINTLN(F("ALERT invalid params"));
            return;
        }

        // user callback
        if (_Alert_app_cb)
            _Alert_app_cb(AlertCode);
    }
    else if (type == 4)
    {
        int LoadCode = (int)param1;
        DEBUG_PRINT(F("LOAD:\t"));
        DEBUG_PRINT(F("code:"));
        DEBUG_PRINTLN(LoadCode);

        bool valid_LoadCode = (LoadCode >= 0 && LoadCode <= 1);
        bool valid_params = valid_LoadCode;
        if (!valid_params)
        {
            DEBUG_PRINTLN(F("LOAD invalid params"));
            return;
        }

        // user callback
        if (_Load_app_cb)
            _Load_app_cb(LoadCode);
    }
    else if (type == 5)
    {
        int BatteryLevel = (int)param1;
        DEBUG_PRINT(F("BATTERY LEVEL:\t"));
        DEBUG_PRINT(F("level:"));
        DEBUG_PRINTLN(BatteryLevel);

        bool valid_BatteryLevel = (BatteryLevel >= 0 && BatteryLevel <= 100);
        bool valid_params = valid_BatteryLevel;
        if (!valid_params)
        {
            DEBUG_PRINTLN(F("BATTERY LEVEL invalid params"));
            return;
        }

        // user callback
        if (_Battery_app_cb)
            _Battery_app_cb(BatteryLevel);
    }
    else
    {
        DEBUG_PRINT(F("UNKNOWN:\t"));
        DEBUG_PRINT(F("Type:"));
        DEBUG_PRINT(type);
        DEBUG_PRINT(F(" || param1:"));
        DEBUG_PRINT(param1);
        DEBUG_PRINT(F(" || param2:"));
        DEBUG_PRINTLN(param2);
    }
}
void PUB_Sensor_cb(rcl_timer_t *timer, int64_t last_call_time)
{
    if (timer != NULL)
    {
        char buffer[150] = {0};
        int size = sizeof(buffer);

        if (_Sensor_app_cb == NULL)
        {
            DEBUG_PRINTLN(F("PUB_Sensor_cb publish  no Sensor_app_cb"));
            return;
        }

        bool app_cb_success = _Sensor_app_cb(buffer, &size) == true;
        if (!app_cb_success)
        {
            DEBUG_PRINTLN(F("PUB_Sensor_cb publish  app_cb failed"));
            return;
        }

        _mSensor.data.data = buffer;
        _mSensor.data.capacity = size;
        bool publish_success = rcl_publish(&_pSensor, &_mSensor, NULL) == RCL_RET_OK;
        if (!publish_success)
        {
            DEBUG_PRINTLN(F("PUB_Sensor_cb publish"));
        }
    }
    else
    {
        DEBUG_PRINTLN(F("PUB_Sensor_cb timer null"));
    }
}
void PUB_AlertSauron_cb(rcl_timer_t *timer, int64_t last_call_time)
{
    if (timer != NULL)
    {
        char buffer[150] = {0};
        int size = sizeof(buffer);

        if (_AlertSauron_app_cb == NULL)
        {
            DEBUG_PRINTLN(F("PUB_AlertSauron_cb publish  no AlertSauron_app_cb"));
            return;
        }

        bool app_cb_success = _AlertSauron_app_cb(buffer, &size) == true;
        if (!app_cb_success)
        {
            DEBUG_PRINTLN(F("PUB_AlertSauron_cb app_cb failed"));
            return;
        }

        _mAlertSauron.data.data = buffer;
        _mAlertSauron.data.capacity = size;
        bool publish_success = rcl_publish(&_pAlertSauron, &_mAlertSauron, NULL) == RCL_RET_OK;
        if (!publish_success)
        {
            DEBUG_PRINTLN(F("PUB_AlertSauron_cb publish"));
        }
    }
    else
    {
        DEBUG_PRINTLN(F("PUB_AlertSauron_cb timer null"));
    }
}

void ros_init(Ros_at_transport_disconnected_t Ros_at_transport_disconnected,
              Ros_at_transport_conencted_t Ros_at_transport_conencted,
              Locomotion_app_cb_t Locomotion_app_cb,
              MaxVelocity_app_cb_t MaxVelocity_app_cb,
              Tower_app_cb_t Tower_app_cb,
              Alert_app_cb_t Alert_app_cb,
              Load_app_cb_t Load_app_cb,
              Battery_app_cb_t Battery_app_cb,
              Sensor_app_cb_t Sensor_app_cb,
              AlertSauron_app_cb_t AlertSauron_app_cb)
{
    // set app callbacks
    _Ros_at_transport_disconnected = Ros_at_transport_disconnected;
    _Ros_at_transport_conencted = Ros_at_transport_conencted;

    // set app callbacks
    _Locomotion_app_cb = Locomotion_app_cb;
    _MaxVelocity_app_cb = MaxVelocity_app_cb;
    _Tower_app_cb = Tower_app_cb;
    _Alert_app_cb = Alert_app_cb;
    _Load_app_cb = Load_app_cb;
    _Battery_app_cb = Battery_app_cb;
    _Sensor_app_cb = Sensor_app_cb;
    _AlertSauron_app_cb = AlertSauron_app_cb;

    // set transport
    Serial.begin(115200);
    set_microros_serial_transports(Serial);

    _Ros_at_transport_disconnected();

    while (Serial.dtr() == 0)
    {
        DEBUG_PRINTLN(F("Esperando conexion a JETSON..."));
        delay(1000);
    }

    _Ros_at_transport_conencted();

    // ros general
    _allocator = rcl_get_default_allocator();
    rclc_support_init(&_support, 0, NULL, &_allocator);
    rclc_node_init_default(&_node, "micro_ros_arduino_node", "", &_support);

    // subscriptions
    rclc_subscription_init_default(&_sMsgToSauron, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/msg");

    // publishers-timers
    rclc_publisher_init_default(&_pSensor, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/tensey/sensor");
    rclc_timer_init_default(&_tSensor, &_support, RCL_MS_TO_NS(1000), PUB_Sensor_cb);

    rclc_publisher_init_default(&_pAlertSauron, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/tensey/alert");
    rclc_timer_init_default(&_tAlertSauron, &_support, RCL_MS_TO_NS(1000), PUB_AlertSauron_cb);

    // executors
    rclc_executor_init(&_eMsgToSauron, &_support.context, 1, &_allocator);
    rclc_executor_add_subscription(&_eMsgToSauron, &_sMsgToSauron, &_mMsgToSauron, &SUB_MsgToSauron_cb, ON_NEW_DATA);

    rclc_executor_init(&_eSensor, &_support.context, 1, &_allocator);
    rclc_executor_add_timer(&_eSensor, &_tSensor);

    rclc_executor_init(&_eAlertSauron, &_support.context, 1, &_allocator);
    rclc_executor_add_timer(&_eAlertSauron, &_tAlertSauron);
}
void ros_loop()
{
    if (rmw_uros_ping_agent(100, 3) != RMW_RET_OK)
    {
        rclc_executor_fini(&_eMsgToSauron);
        rclc_executor_fini(&_eSensor);
        rclc_executor_fini(&_eAlertSauron);

        rcl_publisher_fini(&_pSensor, &_node);
        rcl_publisher_fini(&_pAlertSauron, &_node);
        rcl_subscription_fini(&_sMsgToSauron, &_node);

        rcl_timer_fini(&_tAlertSauron);
        rcl_timer_fini(&_tSensor);

        rcl_node_fini(&_node);
        rclc_support_fini(&_support);

        Serial.end();

        Serial.begin(115200);
        set_microros_serial_transports(Serial);

        _Ros_at_transport_disconnected();

        while (Serial.dtr() == 0)
        {
            DEBUG_PRINTLN(F("Esperando conexion a JETSON..."));
            delay(1000);
        }

        _Ros_at_transport_conencted();

        rclc_support_init(&_support, 0, NULL, &_allocator);
        rclc_node_init_default(&_node, "micro_ros_arduino_node", "", &_support);

        // subscriptions
        rclc_subscription_init_default(&_sMsgToSauron, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/msg");

        // publishers-timers
        rclc_publisher_init_default(&_pSensor, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/tensey/sensor");
        rclc_timer_init_default(&_tSensor, &_support, RCL_MS_TO_NS(1000), PUB_Sensor_cb);

        rclc_publisher_init_default(&_pAlertSauron, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/tensey/alert");
        rclc_timer_init_default(&_tAlertSauron, &_support, RCL_MS_TO_NS(1000), PUB_AlertSauron_cb);

        // executors
        rclc_executor_init(&_eMsgToSauron, &_support.context, 1, &_allocator);
        rclc_executor_add_subscription(&_eMsgToSauron, &_sMsgToSauron, &_mMsgToSauron, &SUB_MsgToSauron_cb, ON_NEW_DATA);

        rclc_executor_init(&_eSensor, &_support.context, 1, &_allocator);
        rclc_executor_add_timer(&_eSensor, &_tSensor);

        rclc_executor_init(&_eAlertSauron, &_support.context, 1, &_allocator);
        rclc_executor_add_timer(&_eAlertSauron, &_tAlertSauron);
    }
    else
    {
        rclc_executor_spin_some(&_eMsgToSauron, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eSensor, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eAlertSauron, RCL_MS_TO_NS(100));
    }
}