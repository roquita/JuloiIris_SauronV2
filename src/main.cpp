#include "Arduino.h"
#include <micro_ros_platformio.h>
#include "std_msgs/msg/string.h"

// #include <stdio.h>
#include <rcl/rcl.h>
// #include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

// Subscriptor 01
rcl_subscription_t subscriber_01;
std_msgs__msg__String input1;
rclc_executor_t executor_sub_01;

// Subscriptor 02
rcl_subscription_t subscriber_02;
std_msgs__msg__String input2;
rclc_executor_t executor_sub_02;

// Publicador 01
rcl_publisher_t publisher_01;
std_msgs__msg__String msg_mcu;
rclc_executor_t executor_pub_01;
rcl_timer_t timer_01;

bool suscription_01_data_r = false;
bool suscription_02_data_r = false;

void subscription_01_callback(const void *msgin)
{
  suscription_01_data_r = true;
  // Serial.println(F("subscription_01_callback"));
}

void subscription_02_callback(const void *msgin)
{
  suscription_02_data_r = true;

  // Serial.println(F("subscription_02_callback"));
}

void timer_01_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  RCLC_UNUSED(last_call_time);
  if (timer != NULL)
  {
    msg_mcu.data.data = "hola mundo";
    msg_mcu.data.size = 11;
    rcl_publish(&publisher_01, &msg_mcu, NULL);
  }
}

void setup()
{
  Serial3.begin(115200);
  Serial3.println(F("prueba"));
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(1000);
  digitalWrite(13, 0);
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
  rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support);

  rclc_subscription_init_default(
      &subscriber_01,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      "/locomotion");

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

  // create executor
  rclc_executor_init(&executor_sub_01, &support.context, 1, &allocator);
  rclc_executor_add_subscription(&executor_sub_01, &subscriber_01, &input1, &subscription_01_callback, ON_NEW_DATA);

  rclc_executor_init(&executor_sub_02, &support.context, 1, &allocator);
  rclc_executor_add_subscription(&executor_sub_02, &subscriber_02, &input2, &subscription_02_callback, ON_NEW_DATA);

  rclc_executor_init(&executor_pub_01, &support.context, 1, &allocator);
  // rclc_executor_add_timer(&executor_pub_01, &timer_01);
}

void loop()
{
  rmw_uros_ping_agent(100, 3);
  rclc_executor_spin_some(&executor_sub_01, RCL_MS_TO_NS(100));
  rclc_executor_spin_some(&executor_sub_02, RCL_MS_TO_NS(100));
  rclc_executor_spin_some(&executor_pub_01, RCL_MS_TO_NS(100));

  if (suscription_01_data_r)
  {
    suscription_01_data_r = false;
    Serial3.println(F("holaaaaa"));

    msg_mcu.data.data = "gaaaaa";
    msg_mcu.data.size = 7;
    rcl_publish(&publisher_01, &msg_mcu, NULL);
  }
  if (suscription_02_data_r)
  {
    suscription_02_data_r = false;
    digitalWrite(13, 1);
    delay(100);
    digitalWrite(13, 0);
    delay(100);
    digitalWrite(13, 1);
    delay(100);
    digitalWrite(13, 0);
  }

  delay(100);
}



/*
#include <Arduino.h>
#include "app/load/load.h"
#include "app/screen/screen.h"
#include "app/sensor/distance.h"
#include "app/sensor/temphumi.h"

#include "project_defines.h"

#include "DFRobot_QMC5883.h"

DFRobot_QMC5883 compass(&Wire2, 0x0D);

void setup()
{
  Serial.begin(115200);

  screen_init();
  screen_goto_MainPage();

  distance_init();
  temphumi_init();
  load_init();
  load_toogling_led();

  while (!compass.begin())
  {
    Serial.println("Could not find a valid 5883 sensor, check wiring!");
    delay(500);
  }
  if (compass.isQMC())
  {
    Serial.println("Initialize QMC5883");
     compass.setRange(QMC5883_RANGE_8GA);
     Serial.print("compass range is:");
     Serial.println(compass.getRange());

     compass.setMeasurementMode(QMC5883_CONTINOUS);
     Serial.print("compass measurement mode is:");
     Serial.println(compass.getMeasurementMode());

     compass.setDataRate(QMC5883_DATARATE_10HZ);
     Serial.print("compass data rate is:");
     Serial.println(compass.getDataRate());

     compass.setSamples(QMC5883_SAMPLES_8);
     Serial.print("compass samples is:");
     Serial.println(compass.getSamples());
  }
}
// int i = 0;

void loop()
{

  //float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
  //compass.setDeclinationAngle(declinationAngle);
  sVector_t mag = compass.readRaw();
  //compass.getHeadingDegrees();
  Serial.print("X:");
  Serial.print(mag.XAxis);
  Serial.print(" Y:");
  Serial.print(mag.YAxis);
  Serial.print(" Z:");
  Serial.println(mag.ZAxis);
 // Serial.print("Degress = ");
  //Serial.println(mag.HeadingDegress);
  delay(1000);

}
*/

