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

// general
rcl_allocator_t _allocator;
rclc_support_t _support;
rcl_node_t _node;

// Subscriptor MsgToSauron
rcl_subscription_t _sMsgToSauron;
geometry_msgs__msg__Twist _mMsgToSauron;
rclc_executor_t _eMsgToSauron;
MsgToSauron_app_cb_t _MsgToSauron_app_cb = NULL;

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

    if (_MsgToSauron_app_cb)
    {
        _MsgToSauron_app_cb(type, param1, param2);
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

void ros_init(MsgToSauron_app_cb_t MsgToSauron_app_cb,
              Sensor_app_cb_t Sensor_app_cb,
              AlertSauron_app_cb_t AlertSauron_app_cb)
{
    // set app callbacks
    _MsgToSauron_app_cb = MsgToSauron_app_cb;
    _Sensor_app_cb = Sensor_app_cb;
    _AlertSauron_app_cb = AlertSauron_app_cb;

    // set transport
    Serial.begin(115200);
    set_microros_serial_transports(Serial);

    while (Serial.dtr() == 0)
    {
        DEBUG_PRINTLN(F("Esperando conexion a JETSON..."));
        delay(1000);
    }

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

        rcl_node_fini(&_node);

        DEBUG_PRINTLN(F("REBOOTING: PING FAILED."));
        delay(1000);
        SCB_AIRCR = 0x05FA0004;
    }
    else
    {
        rclc_executor_spin_some(&_eMsgToSauron, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eSensor, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eAlertSauron, RCL_MS_TO_NS(100));
    }
}