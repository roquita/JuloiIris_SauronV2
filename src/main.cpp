#include "main.h"

void setup()
{
  DEBUG_INIT();
  DEBUG_PRINTLN(F("SETUP IN"));

  screen_init();
  interval_init();
  load_init();
  distance_init();
  temphumi_init();

  wheel_init(wheel_at_start_cb, wheel_at_stop_cb, 
             wheel_MotorFrontRight_at_fault, wheel_MotorFrontRight_at_NoFault,
             wheel_MotorFrontLeft_at_fault, wheel_MotorFrontLeft_at_NoFault,
             wheel_MotorBackRight_at_fault, wheel_MotorBackRight_at_NoFault,
             wheel_MotorBackLeft_at_fault, wheel_MotorBackLeft_at_NoFault);

  ptz_init(ptz_at_start_cb, ptz_at_stop_cb,
           ptz_motor_at_fault, ptz_motor_at_NoFault);

  ros_init(ROS_at_transport_disconnected_cb,
           ROS_at_transport_connected_cb,
           SUB_Locomotion_app_cb,
           SUB_MaxVelocity_app_cb,
           SUB_Tower_app_cb,
           SUB_Alert_app_cb,
           SUB_Load_app_cb,
           SUB_Battery_app_cb,
           PUB_Sensor_app_cb,
           PUB_AlertSauron_app_cb);

  DEBUG_PRINTLN(F("SETUP OUT"));

  load_toogling_led();
  screen_queue_SetupDone_Notificacion();
}

void loop()
{

  if (interval_100ms_triggered())
  {
    ros_loop();
    // distance_loop();
  }

  if (interval_500ms_triggered())
  {
  }

  if (interval_1000ms_triggered())
  {
    temphumi_update();
  }

  if (interval_2000ms_triggered())
  {
    screen_print_DistanceFront(distance_get_Front_mm());
    screen_print_DistanceRightFront(distance_get_RightFront_mm());
    screen_print_DistanceRightMiddle(distance_get_RightMiddle_mm());
    screen_print_DistanceRightBack(distance_get_RightBack_mm());
    screen_print_DistanceLeftFront(distance_get_LeftFront_mm());
    screen_print_DistanceLeftMiddle(distance_get_LeftMiddle_mm());
    screen_print_DistanceLeftBack(distance_get_LeftBack_mm());
    screen_print_DistanceBackRight(distance_get_BackRight_mm());
    screen_print_DistanceBackLeft(distance_get_BackLeft_mm());

    screen_print_EnvironmentTemperature(temphumi_get_temperature());
    screen_print_EnvironmentHumidity(temphumi_get_humidity());
  }

  if (interval_5000ms_triggered())
  {
  }
}