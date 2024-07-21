#include "Arduino.h"
#include "app/interval/interval.h"
#include "app/load/load.h"
#include "app/ros/ros.h"
#include "app/sensor/distance.h"
#include "app/sensor/temphumi.h"
#include "app/screen/screen.h"
#include "project_defines.h"

#include "driver/UART_MUX/uart_mux.h"
#include "driver/DEBUG/debug.h"
#include "driver/XKC-KL200/xkc_kl200.h"

void SUB_Locomotion_app_cb(int direction, int velocity)
{
  // TODO: cambiar cinematica de motores
}
void SUB_MaxVelocity_app_cb(uint32_t MaxVelocity)
{
  // TODO: Guardar nueva maxima velocidad
}
void SUB_Tower_app_cb(int method, int value)
{
  // TODO: Mover torre
}
void SUB_ErrorJetson_app_cb(uint32_t ErrorCode)
{
  if (ErrorCode == 1) // no internet
  {
    load_Toogling_1s_5s_PilotRed();
    screen_queue_NoInternet_Notification();
  }
  else
  {
    DEBUG_PRINT(F("ERROR JETSON UNKNOWN CODE: "));
    DEBUG_PRINTLN(ErrorCode);
  }
}
bool PUB_Sensor_app_cb(char *buffer, int *size)
{

  int bytes = snprintf(buffer, *size, "{"
                                      "\"dis\":[%5i %5i %5i %5i %5i %5i %5i %5i %5i],"
                                      "\"vel\":%3i,"
                                      "\"inc\":%3i,"
                                      "\"ori\":%3i,"
                                      "\"tem\":%3i,"
                                      "\"hum\":%3i"
                                      "}",
                       1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 37, 29, 25, 74);
  bool parsing_success = (bytes > 0) || (bytes < (*size));
  if (!parsing_success)
  {
    DEBUG_PRINTLN(F("PUB_Sensor_app_cb parsing"));
    return false;
  }

  return true;
}
bool PUB_AlertSauron_app_cb(char *buffer, int *size)
{

  int bytes = snprintf(buffer, *size, "{"
                                      "\"dis\":[%1i %1i %1i %1i %1i],"
                                      "\"inc\":%1i,"
                                      "\"lsi\":%1i,"
                                      "\"lsf\":%1i"
                                      "}",
                       0, 0, 3, 1, 0, 0, 0, 0);
  bool parsing_success = (bytes > 0) || (bytes < (*size));
  if (!parsing_success)
  {
    DEBUG_PRINTLN(F("PUB_AlertSauron_app_cb parsing"));
    return false;
  }

  return true;
}

void setup()
{
  Serial.begin(115200);
  //DEBUG_INIT();
  Serial3.begin(9600);
  //DEBUG_PRINTLN(F("SETUP IN"));
 // DEBUG_PRINT(F("\x62\x34\x09\xff\xff\x00\x01\x00\x5f"));
  Serial3.write(0x62);
  Serial3.write(0x34);
  Serial3.write(0x09);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0x00);
  Serial3.write(0x01);
  Serial3.write(0x00);
  Serial3.write(0x5f);
  delay(1000);
  //DEBUG_PRINT(F("\x62\x30\x09\xff\xff\x00\x00\x00\x5b"));
  Serial3.write(0x62);
  Serial3.write(0x30);
  Serial3.write(0x09);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x00);
  Serial3.write(0x5b);

  // screen_init();
  // interval_init();
  // load_init();
  // distance_init();
  // temphumi_init();

  // ros_init(SUB_Locomotion_app_cb, SUB_MaxVelocity_app_cb, SUB_Tower_app_cb, SUB_ErrorJetson_app_cb, PUB_Sensor_app_cb, PUB_AlertSauron_app_cb);

  UARTMUX_init();
  UARTMUX_select_channel(MUX_CHANNEL_14);

 // DEBUG_PRINTLN(F("SETUP OUT"));
}

void loop()
{
  int data = 0;
  xkc_kl200_get_data(&data);
  Serial.println(data);
  delay(1000);
  /*
    if (interval_100ms_triggered())
    {
      ros_loop();
    }

    if (interval_500ms_triggered())
    {
    }
    */
  /*
    if (interval_1000ms_triggered())
    {
      distance_update();
      temphumi_update();

      screen_print_DistanceFront(distance_get_Front() / 10.0);
      screen_print_DistanceRightFront(distance_get_RightFront() / 10.0);
      screen_print_DistanceRightMiddle(distance_get_RightMiddle() / 10.0);
      screen_print_DistanceRightBack(distance_get_RightBack() / 10.0);
      screen_print_DistanceLeftFront(distance_get_LeftFront() / 10.0);
      screen_print_DistanceLeftMiddle(distance_get_LeftMiddle() / 10.0);
      screen_print_DistanceLeftBack(distance_get_LeftBack() / 10.0);
      screen_print_DistanceBackRight(distance_get_BackRight() / 10.0);
      screen_print_DistanceBackLeft(distance_get_BackLeft() / 10.0);

      screen_print_BatteryLevel(50.0);
      screen_print_EnvironmentTemperature(temphumi_get_temperature());
      screen_print_EnvironmentHumidity(temphumi_get_humidity());
    }
  */
  if (interval_2000ms_triggered())
  {
  }

  if (interval_5000ms_triggered())
  {
  }

  /*
  for (int duty = 0; duty < 256; duty += 25)
  {
    analogWrite(DRIVER_5_SV_PIN, duty);
    delay(1000);
  }

  for (int duty = 255; duty > 0; duty -= 25)
  {
    analogWrite(DRIVER_5_SV_PIN, duty);
    delay(1000);
  }
  */

  /*
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
  */
  // Serial3.println(F("1234567890"));
  // delay(100);

  /*

  }
  */

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

  /*
    analogWriteFrequency(DRIVER_5_SV_PIN, 2000.0);
    analogWriteResolution(8);
    analogWrite(DRIVER_5_SV_PIN, 0);

      // analogWriteFrequency(DRIVER_5_SV_PIN, 2000.0);
  // analogWriteResolution(8);
  // analogWrite(DRIVER_5_SV_PIN, 0);
  */
}