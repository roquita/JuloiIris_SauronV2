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
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

// Subscriptor Locomotion
rcl_subscription_t sLocomotion;
geometry_msgs__msg__Twist mLocomotion;
rclc_executor_t eLocomotion;

// Subscirption Velocidad maxima
rcl_subscription_t sMaxVelocity;
std_msgs__msg__UInt32 mMaxVelocity;
rclc_executor_t eMaxVelocity;

// Subscriptor Tower
rcl_subscription_t sTower;
geometry_msgs__msg__Twist mTower;
rclc_executor_t eTower;

// Subscriptor ErrorsJetson
rcl_subscription_t sErrorJetson;
std_msgs__msg__UInt32 mErrorJetson;
rclc_executor_t eErrorJetson;

// Publisher Sensors
rcl_publisher_t pSensor;
std_msgs__msg__String mSensor;
rclc_executor_t eSensor;
rcl_timer_t tSensor;

// Publisher Alert Sauron
rcl_publisher_t pAlertSauron;
std_msgs__msg__String mAlertSauron;
rclc_executor_t eAlertSauron;
rcl_timer_t tAlertSauron;

void sLocomotion_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("sLocomotion_cb"));

    const geometry_msgs__msg__Twist *mLocomotion = (const geometry_msgs__msg__Twist *)msgin;
    DEBUG_PRINT(F("Direccion: "));
    DEBUG_PRINTLN(mLocomotion->linear.x);
    DEBUG_PRINT(F("Rapidez: "));
    DEBUG_PRINTLN(mLocomotion->linear.y);
}
void sMaxVelocity_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("sMaxVelocity_cb"));

    const std_msgs__msg__UInt32 *mMaxVelocity = (const std_msgs__msg__UInt32 *)msgin;
    DEBUG_PRINT(F("Velocidad maxima(%): "));
    DEBUG_PRINTLN(mMaxVelocity->data);
}
void sTower_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("sTower_cb"));

    const geometry_msgs__msg__Twist *mTower = (const geometry_msgs__msg__Twist *)msgin;
    DEBUG_PRINT(F("Metodo: "));
    DEBUG_PRINTLN(mTower->linear.x);
    DEBUG_PRINT(F("Valor: "));
    DEBUG_PRINTLN(mTower->linear.y);
}
void sErrorJetson_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("sErrorJetson_cb"));

    const std_msgs__msg__UInt32 *mErrorsJetson = (const std_msgs__msg__UInt32 *)msgin;
    DEBUG_PRINT(F("Error Jetson code: "));
    DEBUG_PRINTLN(mErrorsJetson->data);
}
void tSensor_cb(rcl_timer_t *timer, int64_t last_call_time)
{
    if (timer != NULL)
    {
        char buffer[150] = {0};
        int bytes = snprintf(buffer, sizeof(buffer), "{"
                                                     "\"dis\":[%5i %5i %5i %5i %5i %5i %5i %5i %5i],"
                                                     "\"vel\":%3i,"
                                                     "\"inc\":%3i,"
                                                     "\"ori\":%3i,"
                                                     "\"tem\":%3i,"
                                                     "\"hum\":%3i"
                                                     "}",
                             1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 37, 29, 25, 74);
        bool parsing_success = (bytes > 0) || (bytes < (int)sizeof(buffer));
        if (!parsing_success)
        {
            DEBUG_PRINTLN(F("tSensor_cb parsing"));
            return;
        }

        mSensor.data.data = buffer;
        mSensor.data.capacity = bytes + 1;
        bool publish_success = rcl_publish(&pSensor, buffer, NULL) == RCL_RET_OK;
        if (!publish_success)
        {
            DEBUG_PRINTLN(F("tSensor_cb publish"));
        }
    }
}
void tAlertSauron_cb(rcl_timer_t *timer, int64_t last_call_time)
{
    if (timer != NULL)
    {
        char buffer[150] = {0};
        int bytes = snprintf(buffer, sizeof(buffer), "{"
                                                     "\"dis\":[%1i %1i %1i %1i %1i],"
                                                     "\"inc\":%1i,"
                                                     "\"lsi\":%1i,"
                                                     "\"lsf\":%1i"
                                                     "}",
                             0, 0, 3, 1, 0, 0, 0, 0);
        bool parsing_success = (bytes > 0) || (bytes < (int)sizeof(buffer));
        if (!parsing_success)
        {
            DEBUG_PRINTLN(F("tAlertSauron_cb parsing"));
            return;
        }

        mAlertSauron.data.data = buffer;
        mAlertSauron.data.capacity = bytes + 1;
        bool publish_success = rcl_publish(&pAlertSauron, buffer, NULL) == RCL_RET_OK;
        if (!publish_success)
        {
            DEBUG_PRINTLN(F("tAlertSauron_cb publish"));
        }
    }
}

void ros_init(suscriber_cb_t suscriber01_cb, suscriber_cb_t suscriber02_cb, publiser_cb_t publisher01_cb, publiser_cb_t publisher02_cb)
{

    // set transport
    Serial.begin(115200);
    set_microros_serial_transports(Serial);

    while (Serial.dtr() == 0)
    {
        DEBUG_PRINTLN(F("Esperando conexion a JETSON..."));
        delay(1000);
    }

    // ros general
    allocator = rcl_get_default_allocator();
    rclc_support_init(&support, 0, NULL, &allocator);
    rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support);

    // subscriptions
    rclc_subscription_init_default(&sLocomotion, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/locomotion");
    rclc_subscription_init_default(&sMaxVelocity, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt32), "/tensey/velocity");
    rclc_subscription_init_default(&sTower, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/tower");
    rclc_subscription_init_default(&sErrorJetson, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt32), "/tensey/errorjetson");

    // publishers-timers
    rclc_publisher_init_default(&pSensor, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/sensor");
    rclc_timer_init_default(&tSensor, &support, RCL_MS_TO_NS(1000), tSensor_cb);

    rclc_publisher_init_default(&pAlertSauron, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), "/alertsauron");
    rclc_timer_init_default(&tAlertSauron, &support, RCL_MS_TO_NS(1000), tAlertSauron_cb);

    // executors
    rclc_executor_init(&eLocomotion, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&eLocomotion, &sLocomotion, &mLocomotion, &sLocomotion_cb, ON_NEW_DATA);

    rclc_executor_init(&eMaxVelocity, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&eMaxVelocity, &sMaxVelocity, &mMaxVelocity, &sMaxVelocity_cb, ON_NEW_DATA);

    rclc_executor_init(&eTower, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&eTower, &sTower, &mTower, &sTower_cb, ON_NEW_DATA);

    rclc_executor_init(&eErrorJetson, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&eErrorJetson, &sErrorJetson, &mErrorJetson, &sErrorJetson_cb, ON_NEW_DATA);

    rclc_executor_init(&eSensor, &support.context, 1, &allocator);
    rclc_executor_add_timer(&eSensor, &tSensor);

    rclc_executor_init(&eAlertSauron, &support.context, 1, &allocator);
    rclc_executor_add_timer(&eAlertSauron, &tAlertSauron);
}
void ros_loop()
{
    if (rmw_uros_ping_agent(100, 3) != RMW_RET_OK)
    {
        SCB_AIRCR = 0x05FA0004;
    }
    else
    {
        rclc_executor_spin_some(&eLocomotion, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&eMaxVelocity, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&eTower, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&eErrorJetson, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&eSensor, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&eAlertSauron, RCL_MS_TO_NS(100));
    }
}