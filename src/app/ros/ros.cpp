#include "Arduino.h"
#include "ros.h"
#include "../../driver/DEBUG/debug.h"
#include "../load/load.h"

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include "std_msgs/msg/string.h"
// #include <stdio.h>
#include <rcl/rcl.h>
// #include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/u_int32.h>
#include <geometry_msgs/msg/twist.h>

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

// Subscriptor 01
rcl_subscription_t subscriber_01;
char input1_buffer[100];
std_msgs__msg__String input1;
rclc_executor_t executor_sub_01;

/*
// Subscriptor 02
rcl_subscription_t subscriber_02;
std_msgs__msg__String input2;
rclc_executor_t executor_sub_02;

// Publicador 01
rcl_publisher_t publisher_01;
std_msgs__msg__String msg_mcu;
rclc_executor_t executor_pub_01;
rcl_timer_t timer_01;
*/
void subscription_01_callback(const void *msgin)
{
    DEBUG_PRINTLN(F("subscription_01_callback"));

    load_TurnOff_led();
}

void subscription_02_callback(const void *msgin)
{

    DEBUG_PRINTLN(F("subscription_02_callback"));
}

void timer_01_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    /*
    RCLC_UNUSED(last_call_time);
    if (timer != NULL)
    {
        msg_mcu.data.data = "hola mundo";
        msg_mcu.data.size = 11;
        rcl_publish(&publisher_01, &msg_mcu, NULL);
    }
    */
}

void ros_init(suscriber_cb_t suscriber01_cb, suscriber_cb_t suscriber02_cb, publiser_cb_t publisher01_cb, publiser_cb_t publisher02_cb)
{

    Serial.begin(115200);
    set_microros_serial_transports(Serial);

    while (Serial.dtr() == 0)
    {
        digitalWrite(13, 1);
        delay(1000);
        digitalWrite(13, 0);
        delay(1000);
    }

    allocator = rcl_get_default_allocator();

    // create init_options
    rclc_support_init(&support, 0, NULL, &allocator);

    // create node
    rclc_node_init_default(&node, "sauron v2", "iris", &support);

    rclc_subscription_init_default(
        &subscriber_01,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
        "/tensey/locomotion");
    /*
        // msg hace referencia al tipo de mensaje y no al monbre del mensaje

        // create subscriber_02

        rclc_subscription_init_default(
            &subscriber_02,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
            "/tower");

        // Creador publicador
        rclc_publisher_init_default(
            &publisher_01,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
            "/mcu");

        // create timer, called every 1000 ms to publish heartbeat
        const unsigned int timer_01_timeout = 1000;
        rclc_timer_init_default(
            &timer_01,
            &support,
            RCL_MS_TO_NS(timer_01_timeout),
            timer_01_callback);
    */

    // create executor
    input1.data.data = input1_buffer;
    input1.data.capacity = 100;
    rclc_executor_init(&executor_sub_01, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&executor_sub_01, &subscriber_01, &input1, &subscription_01_callback, ON_NEW_DATA);

    /*
        rclc_executor_init(&executor_sub_02, &support.context, 1, &allocator);
        rclc_executor_add_subscription(&executor_sub_02, &subscriber_02, &input2, &subscription_02_callback, ON_NEW_DATA);

        rclc_executor_init(&executor_pub_01, &support.context, 1, &allocator);
        rclc_executor_add_timer(&executor_pub_01, &timer_01);
        */
}
void ros_loop()
{
    rclc_executor_spin_some(&executor_sub_01, RCL_MS_TO_NS(100));
    delay(100);
}