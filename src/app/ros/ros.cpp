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

// Subscriptor Locomotion
rcl_subscription_t _sLocomotion;
geometry_msgs__msg__Twist _mLocomotion;
rclc_executor_t _eLocomotion;
Locomotion_app_cb_t _Locomotion_app_cb = NULL;

// Subscirption Velocidad maxima
rcl_subscription_t _sMaxVelocity;
std_msgs__msg__UInt32 _mMaxVelocity;
rclc_executor_t _eMaxVelocity;
MaxVelocity_app_cb_t _MaxVelocity_app_cb = NULL;

// Subscriptor Tower
rcl_subscription_t _sTower;
geometry_msgs__msg__Twist _mTower;
rclc_executor_t _eTower;
Tower_app_cb_t _Tower_app_cb = NULL;

// Subscriptor ErrorsJetson
rcl_subscription_t _sErrorJetson;
std_msgs__msg__UInt32 _mErrorJetson;
rclc_executor_t _eErrorJetson;
ErrorJetson_app_cb_t _ErrorJetson_app_cb = NULL;

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

void SUB_Locomotion_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("SUB_Locomotion_cb"));

    const geometry_msgs__msg__Twist *mLocomotion = (const geometry_msgs__msg__Twist *)msgin;
    DEBUG_PRINT(F("Direccion: "));
    DEBUG_PRINTLN(mLocomotion->linear.x);
    DEBUG_PRINT(F("Rapidez: "));
    DEBUG_PRINTLN(mLocomotion->linear.y);

    int direction = (int)mLocomotion->linear.x;
    int velocity = (int)mLocomotion->linear.y;

    if (_Locomotion_app_cb)
    {
        _Locomotion_app_cb(direction, velocity);
    }
}
void SUB_MaxVelocity_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("SUB_MaxVelocity_cb"));

    const std_msgs__msg__UInt32 *mMaxVelocity = (const std_msgs__msg__UInt32 *)msgin;
    DEBUG_PRINT(F("Velocidad maxima(%): "));
    DEBUG_PRINTLN(mMaxVelocity->data);

    if (_MaxVelocity_app_cb)
    {
        _MaxVelocity_app_cb(mMaxVelocity->data);
    }
}
void SUB_Tower_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("SUB_Tower_cb"));

    const geometry_msgs__msg__Twist *mTower = (const geometry_msgs__msg__Twist *)msgin;
    DEBUG_PRINT(F("Metodo: "));
    DEBUG_PRINTLN(mTower->linear.x);
    DEBUG_PRINT(F("Valor: "));
    DEBUG_PRINTLN(mTower->linear.y);

    int method = (int)mTower->linear.x;
    int value = (int)mTower->linear.y;

    if (_Tower_app_cb)
    {
        _Tower_app_cb(method, value);
    }
}
void SUB_ErrorJetson_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("SUB_ErrorJetson_cb"));

    const std_msgs__msg__UInt32 *mErrorsJetson = (const std_msgs__msg__UInt32 *)msgin;
    DEBUG_PRINT(F("Error Jetson code: "));
    DEBUG_PRINTLN(mErrorsJetson->data);

    uint32_t ErrorCode = (uint32_t)mErrorsJetson->data;

    if (_ErrorJetson_app_cb)
    {
        _ErrorJetson_app_cb(ErrorCode);
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
        _mSensor.data.capacity = size + 1;
        bool publish_success = rcl_publish(&_pSensor, buffer, NULL) == RCL_RET_OK;
        if (!publish_success)
        {
            DEBUG_PRINTLN(F("PUB_Sensor_cb publish"));
        }
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
        _mAlertSauron.data.capacity = size + 1;
        bool publish_success = rcl_publish(&_pAlertSauron, buffer, NULL) == RCL_RET_OK;
        if (!publish_success)
        {
            DEBUG_PRINTLN(F("PUB_AlertSauron_cb publish"));
        }
    }
}

void ros_init(Locomotion_app_cb_t Locomotion_app_cb,
              MaxVelocity_app_cb_t MaxVelocity_app_cb,
              Tower_app_cb_t Tower_app_cb,
              ErrorJetson_app_cb_t ErrorJetson_app_cb,
              Sensor_app_cb_t Sensor_app_cb,
              AlertSauron_app_cb_t AlertSauron_app_cb)
{
    // set app callbacks
    _Locomotion_app_cb = Locomotion_app_cb;
    _MaxVelocity_app_cb = MaxVelocity_app_cb;
    _Tower_app_cb = Tower_app_cb;
    _ErrorJetson_app_cb = ErrorJetson_app_cb;
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
    rclc_subscription_init_default(&_sLocomotion, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/locomotion");
    rclc_subscription_init_default(&_sMaxVelocity, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt32), "/tensey/velocity");
    rclc_subscription_init_default(&_sTower, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/tower");
    rclc_subscription_init_default(&_sErrorJetson, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt32), "/tensey/errorjetson");

    // publishers-timers
    rclc_publisher_init_default(&_pSensor, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/sensor");
    rclc_timer_init_default(&_tSensor, &_support, RCL_MS_TO_NS(1000), PUB_Sensor_cb);

    rclc_publisher_init_default(&_pAlertSauron, &_node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/alertsauron");
    rclc_timer_init_default(&_tAlertSauron, &_support, RCL_MS_TO_NS(1000), PUB_AlertSauron_cb);

    // executors
    rclc_executor_init(&_eLocomotion, &_support.context, 1, &_allocator);
    rclc_executor_add_subscription(&_eLocomotion, &_sLocomotion, &_mLocomotion, &SUB_Locomotion_cb, ON_NEW_DATA);

    rclc_executor_init(&_eMaxVelocity, &_support.context, 1, &_allocator);
    rclc_executor_add_subscription(&_eMaxVelocity, &_sMaxVelocity, &_mMaxVelocity, &SUB_MaxVelocity_cb, ON_NEW_DATA);

    rclc_executor_init(&_eTower, &_support.context, 1, &_allocator);
    rclc_executor_add_subscription(&_eTower, &_sTower, &_mTower, &SUB_Tower_cb, ON_NEW_DATA);

    rclc_executor_init(&_eErrorJetson, &_support.context, 1, &_allocator);
    rclc_executor_add_subscription(&_eErrorJetson, &_sErrorJetson, &_mErrorJetson, &SUB_ErrorJetson_cb, ON_NEW_DATA);

    rclc_executor_init(&_eSensor, &_support.context, 1, &_allocator);
    rclc_executor_add_timer(&_eSensor, &_tSensor);

    rclc_executor_init(&_eAlertSauron, &_support.context, 1, &_allocator);
    rclc_executor_add_timer(&_eAlertSauron, &_tAlertSauron);
}
void ros_loop()
{
    if (rmw_uros_ping_agent(100, 3) != RMW_RET_OK)
    {
        SCB_AIRCR = 0x05FA0004;
    }
    else
    {
        rclc_executor_spin_some(&_eLocomotion, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eMaxVelocity, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eTower, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eErrorJetson, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eSensor, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&_eAlertSauron, RCL_MS_TO_NS(100));
    }
}