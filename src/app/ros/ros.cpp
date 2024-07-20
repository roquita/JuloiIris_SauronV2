#include "Arduino.h"
#include "ros.h"
#include "../../driver/DEBUG/debug.h"
#include "../load/load.h"

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include "std_msgs/msg/string.h"
#include "std_msgs/msg/header.h"
// #include <stdio.h>
#include <rcl/rcl.h>
// #include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/u_int32.h>
#include <geometry_msgs/msg/twist.h>

// Todo es /teensy/x

//********************************* */ SUSCRIPCIONES  *********************//
// TOPICO LOCOMOCION
// linear
// x = 0 => parar
// x =  1  => adelante
// x = 2 => atras
// x = 3 => girar a derecha
// x = 4 => girar a izquierda
// y [0.0:1.0]  => rapidez

// TOPICO VELOCIDAD
// velocida maxima
// 1 entero  [0-100%]

// TOPICO TORRE
// torre
// x = 1 => avanzar por pulsos
// x = 2 => avanzar por limite
// x = 3 => avanzar por posicion
// y = (si x =1) -1 o +1
// y = (si x =2) -1 o +1
// y = (si x= 3)  [0-100%]

// TOPICO ERRORES JETSON
// entero
// u = 1 => no internet. Bloquear locomocion y parar. Piloto rojo parpadeo cada 1s-5s.
//          Notificacion nextion en rojo.

//********************************* */ PUBLICADORES  *********************//

// TOPICO SENSORES, timer 1s
// string
// distancia[1-9], velocidad del carro, inclinacionm, orientacion, temperatura, humedad

// TOPICO ALERTAS SAURON
// String
// drivers[1-8], inclinacion peligrosa,  final de carrera inicial estado, final de carrera final estado

// general
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

// Subscriptor Locomotion
rcl_subscription_t sLocomotion;
geometry_msgs__msg__Twist mLocomotion;
rclc_executor_t elocomotion;

// 

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

void sLocomotion_cb(const void *msgin)
{
    DEBUG_PRINTLN(F("sLocomotion_cb"));

    const geometry_msgs__msg__Twist *mLocomotion = (const geometry_msgs__msg__Twist *)msgin;
    DEBUG_PRINT(F("Direccion: "));
    DEBUG_PRINTLN(mLocomotion->linear.x);
    DEBUG_PRINT(F("Rapidez: "));
    DEBUG_PRINTLN(mLocomotion->linear.y);
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
        // digitalWrite(13, 1);
        // delay(1000);
        // digitalWrite(13, 0);
        DEBUG_PRINTLN(F("Esperando conexion a JETSON..."));
        delay(1000);
    }

    allocator = rcl_get_default_allocator();

    // create init_options
    rclc_support_init(&support, 0, NULL, &allocator);

    // create node
    rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support);
    rclc_subscription_init_default(&sLocomotion, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "/tensey/locomotion");

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

    rclc_executor_init(&elocomotion, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&elocomotion, &sLocomotion, &mLocomotion, &sLocomotion_cb, ON_NEW_DATA);

    /*
        rclc_executor_init(&executor_sub_02, &support.context, 1, &allocator);
        rclc_executor_add_subscription(&executor_sub_02, &subscriber_02, &input2, &subscription_02_callback, ON_NEW_DATA);

        rclc_executor_init(&executor_pub_01, &support.context, 1, &allocator);
        rclc_executor_add_timer(&executor_pub_01, &timer_01);
        */
}
void ros_loop()
{
    if (rmw_uros_ping_agent(100, 3) != RMW_RET_OK)
    {
        SCB_AIRCR = 0x05FA0004;
    }
    else
    {
        rclc_executor_spin_some(&elocomotion, RCL_MS_TO_NS(100));
    }
}