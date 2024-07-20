#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/u_int32.h>
#include <std_msgs/msg/string.h>
#include <geometry_msgs/msg/twist.h>

#define RCCHECK(fn)              \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
      error_loop();              \
    }                            \
  }
#define RCSOFTCHECK(fn)          \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
    }                            \
  }

#define RCLOOPCHECK(fn)          \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
      error_loop_loop();         \
    }                            \
  }


#define PERIODO_00 100
#define PERIODO_01 200
#define PERIODO_02 500
uint32_t cont_00 = 0;
uint32_t cont_01 = 0;
uint32_t cont_02 = 0;

rcl_allocator_t allocator;
rclc_support_t support;

rcl_node_t node;

// Subscriptor 01
rcl_subscription_t subscriber_01;
geometry_msgs__msg__Twist msg_01;
rclc_executor_t executor_sub_01;

// Publicador 01
rcl_publisher_t publisher_01;
std_msgs__msg__String msg_mcu;
rclc_executor_t executor_pub_01;
rcl_timer_t timer_01;

uint8_t f_error_ros = 0;

/**
 *
 */
void error_loop()
{
  while (1)
  {
    digitalWrite(13, !digitalRead(13));
    delay(100);
  }
}

/**
 *
 */
void timer_01_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  RCLC_UNUSED(last_call_time);
  if (timer != NULL)
  {
    msg_mcu.data.data = "hola mundo";
    msg_mcu.data.size = 11;
    RCSOFTCHECK(rcl_publish(&publisher_01, &msg_mcu, NULL));
  }
}


/**
 * @brief Subcriptor: Funcion a ejecutarse cuando se recibe un mensaje
 */

void subscription_01_callback(const void *msgin)
{
  const geometry_msgs__msg__Twist *msg_01 = (const geometry_msgs__msg__Twist *)msgin;
  digitalWrite(13, (msg_01->linear.x == 0) ? LOW : HIGH);
}

/**
 *
 */
void init_nodo(void)
{
  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));
}

/**
 *
 */
void init_plublicador_subcriptor()
{
  // create subscriber_01

  RCCHECK(rclc_subscription_init_default(
      &subscriber_01,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      "/tensey/locomotion"));

  // msg hace referencia al tipo de mensaje y no al monbre del mensaje


  // msg hace referencia al tipo de mensaje y no al monbre del mensaje

  // Creador publicador
  RCCHECK(rclc_publisher_init_default(
      &publisher_01,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      "/tensey/mcu"));

  // create timer, called every 1000 ms to publish heartbeat
  const unsigned int timer_01_timeout = 1000;
  RCCHECK(rclc_timer_init_default(
      &timer_01,
      &support,
      RCL_MS_TO_NS(timer_01_timeout),
      timer_01_callback));

}

/**
 *
 */
void init_ejecutor(void)
{
  // create executor
  RCCHECK(rclc_executor_init(&executor_sub_01, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor_sub_01, &subscriber_01, &msg_01, &subscription_01_callback, ON_NEW_DATA));

  RCCHECK(rclc_executor_init(&executor_pub_01, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor_pub_01, &timer_01));
}

/**
 *
 */
void fini_publicador_subcriptor(void)
{
  rcl_publisher_fini(&publisher_01, &node);
  rcl_subscription_fini(&subscriber_01, &node);
}

/**
 *
 */
void fini_nodo(void)
{
  rcl_node_fini(&node);
}

/**
 *
 */
void error_loop_loop()
{
  while (f_error_ros)
  {
    while (Serial.dtr() == 0)
    {
      digitalWrite(13, 1);
      delay(1000);
      digitalWrite(13, 0);
      delay(1000);
    }
    f_error_ros = 0;
  }
  fini_publicador_subcriptor();
  fini_nodo();

  delay(100);

  init_nodo();
  init_plublicador_subcriptor();
  init_ejecutor();
}

/**
 *
 */
void setup()
{

  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(1000);
  digitalWrite(13, 0);

  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  while (Serial.dtr() == 0)
  {
    digitalWrite(13, 1);
    delay(1000);
    digitalWrite(13, 0);
    delay(1000);
  }

  delay(2000);

  init_nodo();

  init_plublicador_subcriptor();

  init_ejecutor();

}

void loop()
{
  if ((millis() - cont_00) >= PERIODO_00)
  {

    if (rmw_uros_ping_agent(100, 3) != RMW_RET_OK)
    {
      SCB_AIRCR = 0x05FA0004;
    }
    else
    {
      RCLOOPCHECK(rclc_executor_spin_some(&executor_sub_01, RCL_MS_TO_NS(100)));
      RCLOOPCHECK(rclc_executor_spin_some(&executor_pub_01, RCL_MS_TO_NS(100)));
    }
    cont_00 = millis();
  }
}