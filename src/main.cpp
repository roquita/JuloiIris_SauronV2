#include "Arduino.h"
#include "app/interval/interval.h"
#include "app/load/load.h"
#include "app/ros/ros.h"
#include "app/sensor/distance.h"
#include "app/sensor/temphumi.h"
#include "app/screen/screen.h"
#include "app/wheel/wheel.h"
#include "app/ptz/ptz.h"
#include "project_defines.h"

#include "driver/UART_MUX/uart_mux.h"
#include "driver/DEBUG/debug.h"
#include "driver/XKC-KL200/xkc_kl200.h"
#include "driver/BLD_300B/bld_300b.h"

void SUB_MsgToSauron_app_cb(int type, float param1, float param2)
{
  switch (type)
  {
  case 0: // LOCOMOTION
  {
    DEBUG_PRINTLN(F("SUB_MsgToSauron_app_cb LOCOMOTION"));
    int direction = (int)param1;
    float velocity = param2;
    DEBUG_PRINT(F("Direccion: "));
    DEBUG_PRINTLN(direction);
    DEBUG_PRINT(F("Rapidez: "));
    DEBUG_PRINTLN(velocity);

    bool valid_direction = (direction >= 0 && direction <= 4);
    bool valid_velocity = (velocity >= 0.0 && velocity <= 1.0);
    bool valid_params = valid_direction && valid_velocity;
    if (!valid_params)
    {
      DEBUG_PRINTLN(F("LOCOMOTION invalid params"));
      break;
    }

    switch (direction)
    {
    case 0: // PARAR
      wheel_stop();
      break;
    case 1: // ADELANTE
      wheel_move_forward(velocity);
      break;
    case 2: // ATRAS
      wheel_move_backward(velocity);
      break;
    case 3: // GIRAR DERECHA
      wheel_turn_right(velocity);
      break;
    default: // GIRAR IZQUIERDA
      wheel_turn_left(velocity);
      break;
    }
  }
  break;
  case 1: // MAX VELOCITY
  {
    DEBUG_PRINTLN(F("SUB_MsgToSauron_app_cb MAX VELOCITY"));
    int MaxVelocity = (int)param1;
    DEBUG_PRINT(F("Velocidad maxima(%): "));
    DEBUG_PRINTLN(MaxVelocity);

    bool valid_MaxVelocity = (MaxVelocity >= 0 && MaxVelocity <= 100);
    bool valid_params = valid_MaxVelocity;
    if (!valid_params)
    {
      DEBUG_PRINTLN(F("MAX VELOCITY invalid params"));
      break;
    }

    wheel_set_MaxSpeed(MaxVelocity);
  }
  break;
  case 2: // TOWER
  {
    DEBUG_PRINTLN(F("SUB_MsgToSauron_app_cb TOWER"));
    int method = (int)param1;
    int value = (int)param2;
    DEBUG_PRINT(F("Metodo: "));
    DEBUG_PRINTLN(method);
    DEBUG_PRINT(F("Valor: "));
    DEBUG_PRINTLN(value);

    bool valid_method_by_pulse = (method == 1 && (value == 1 || value == -1));
    bool valid_method_by_limit = (method == 2 && (value == 1 || value == -1));
    bool valid_mehotd_by_pos = (method == 3 && (value >= 0 && value <= 100));
    bool valid_params = valid_method_by_pulse || valid_method_by_limit || valid_mehotd_by_pos;
    if (!valid_params)
    {
      DEBUG_PRINTLN(F("TOWER invalid params"));
      break;
    }

    switch (method)
    {
    case 1: // BY PULSE
      value == 1 ? ptz_MoveUp_by_pulse() : ptz_MoveDown_by_pulse();
      break;
    case 2: // BY LIMIT
      value == 1 ? ptz_MoveUp_by_limit() : ptz_MoveDown_by_limit();
      break;
    default: // BY POSITION
      ptz_move_by_position(value);
      break;
    }
  }
  break;
  case 3: // ALERT JETSON
  {
    DEBUG_PRINTLN(F("SUB_MsgToSauron_app_cb ALERT JETSON"));
    int ErrorCode = (int)param1;
    DEBUG_PRINT(F("Error Jetson code: "));
    DEBUG_PRINTLN(ErrorCode);

    if (ErrorCode == 1) // no internet
    {
      load_Toogling_1s_5s_PilotRed();
      screen_queue_NoInternet_Notification();
    }
  }
  break;

  default: // UNKNOWN
    DEBUG_PRINT(F("SUB_MsgToSauron_app_cb: unknown type "));
    DEBUG_PRINT(type);
    break;
  }
}

bool PUB_Sensor_app_cb(char *buffer, int *size)
{
  int bytes = snprintf(buffer, *size, "\"{"
                                      "'dis':[%i,%i,%i,%i,%i,%i,%i,%i,%i],"
                                      "'vel':%i,"
                                      "'inc':%i,"
                                      "'ori':%i,"
                                      "'tem':%i,"
                                      "'hum':%i"
                                      "}\"",
                       1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 37, 29, 25, 74);
  bool parsing_success = (bytes > 0) || (bytes < (*size));
  *size = bytes + 1;
  if (!parsing_success)
  {
    DEBUG_PRINTLN(F("PUB_Sensor_app_cb parsing"));
    return false;
  }

  return true;
}
bool PUB_AlertSauron_app_cb(char *buffer, int *size)
{
  // DEBUG_PRINTLN(F("PUB_AlertSauron_app_cb"));

  int bytes = snprintf(buffer, *size, "\"{"
                                      "'dis':[%1i,%1i,%1i,%1i,%1i],"
                                      "'inc':%1i,"
                                      "'lsi':%1i,"
                                      "'lsf':%1i"
                                      "\"}",
                       0, 0, 3, 1, 0, 0, 0, 0);
  bool parsing_success = (bytes > 0) || (bytes < (*size));
  *size = bytes + 1;
  if (!parsing_success)
  {
    DEBUG_PRINTLN(F("PUB_AlertSauron_app_cb parsing"));
    return false;
  }

  return true;
}

void setup()
{
  // delay(10000);
  DEBUG_INIT();
  DEBUG_PRINTLN(F("SETUP IN"));

  // screen_init();
  // interval_init();
  // load_init();
  // distance_init();
  // temphumi_init();
   wheel_init();

  /*
    analogWriteResolution(8);
    pinMode(DRIVER_5_FR_PIN, OUTPUT);
    digitalWrite(DRIVER_5_FR_PIN, 0);

    analogWriteFrequency(DRIVER_5_SV_PIN, 2000.0);
    analogWrite(DRIVER_5_SV_PIN, 128); // 0=100% - 255=0%
    */

  // ros_init(SUB_MsgToSauron_app_cb, PUB_Sensor_app_cb, PUB_AlertSauron_app_cb);

  DEBUG_PRINTLN(F("SETUP OUT"));
}

void loop()
{

  if (interval_100ms_triggered())
  {
    // ros_loop();
  }

  if (interval_500ms_triggered())
  {
  }

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
}