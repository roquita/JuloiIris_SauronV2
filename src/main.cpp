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
#include "driver/NEXTION/nextion.h"

void SUB_MsgToSauron_app_cb(int type, float param1, float param2)
{
  switch (type)
  {
  case 0: // LOCOMOTION
  {
    DEBUG_PRINTLN(F("LOCOMOTION"));
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
      SCREEN_DEBUG("LOCOMOTION invalid params");
      break;
    }

    switch (direction)
    {
    case 0: // PARAR
      SCREEN_DEBUG("PARAR");
      wheel_stop();
      break;
    case 1: // ADELANTE
      SCREEN_DEBUG("ADELANTE");
      wheel_move_forward(velocity);
      break;
    case 2: // ATRAS
      SCREEN_DEBUG("ATRAS ");
      wheel_move_backward(velocity);
      break;
    case 3: // GIRAR DERECHA
      SCREEN_DEBUG("GIRAR DERECHA");
      wheel_turn_right(velocity);
      break;
    default: // GIRAR IZQUIERDA
      SCREEN_DEBUG("GIRAR IZQUIERDA");
      wheel_turn_left(velocity);
      break;
    }
  }
  break;
  case 1: // MAX VELOCITY
  {
    DEBUG_PRINTLN(F("MAX VELOCITY"));
    int MaxVelocity = (int)param1;
    DEBUG_PRINT(F("Velocidad maxima(%): "));
    DEBUG_PRINTLN(MaxVelocity);

    bool valid_MaxVelocity = (MaxVelocity >= 0 && MaxVelocity <= 100);
    bool valid_params = valid_MaxVelocity;
    if (!valid_params)
    {
      DEBUG_PRINTLN(F("MAX VELOCITY invalid params"));
      SCREEN_DEBUG("MAX VELOCITY invalid params");
      break;
    }
    SCREEN_DEBUG("MAX VELOCITY");
    wheel_set_MaxSpeed(MaxVelocity);
  }
  break;
  case 2: // TOWER
  {
    DEBUG_PRINTLN(F("TOWER"));
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
      // value == 1 ? ptz_MoveUp_by_pulse() : ptz_MoveDown_by_pulse();
      break;
    case 2: // BY LIMIT
      // value == 1 ? ptz_MoveUp_by_limit() : ptz_MoveDown_by_limit();
      break;
    default: // BY POSITION
      // ptz_move_by_position(value);
      break;
    }
  }
  break;
  case 3: // ALERT
  {
    DEBUG_PRINTLN(F("ALERT"));
    int ErrorCode = (int)param1;
    DEBUG_PRINT(F("code: "));
    DEBUG_PRINTLN(ErrorCode);

    bool valid_ErrorCode = (ErrorCode >= 1 && ErrorCode <= 3);
    bool valid_params = valid_ErrorCode;
    if (!valid_params)
    {
      DEBUG_PRINTLN(F("ALERT invalid params"));
      break;
    }

    switch (ErrorCode)
    {
    case 1: // NO INTERNET
      wheel_stop();
      // ptz_stop();
      // load_Toogling_1s_5s_PilotRed();
      screen_queue_NoInternet_Notification();
      break;
    case 2: // SI INTERNET
      screen_goto_MainPage();
      screen_queue_YesInternet_Notificacion();
      break;
    default: // BATERIA BAJA
      screen_queue_LowBatery_Notificacion();
      break;
    }
  }
  break;
  case 4: // LOAD
  {
    DEBUG_PRINTLN(F("LOAD"));
    int LoadCode = (int)param1;
    DEBUG_PRINT(F("code: "));
    DEBUG_PRINTLN(LoadCode);

    bool valid_LoadCode = (LoadCode >= 0 && LoadCode <= 1);
    bool valid_params = valid_LoadCode;
    if (!valid_params)
    {
      DEBUG_PRINTLN(F("LOAD invalid params"));
      break;
    }

    switch (LoadCode)
    {
    case 0: //  APAGAR LUCES
      // load_TurnOff_light();
      break;
    default: // ENCENDER LUCES
      // load_TurnOn_light();
      break;
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
                                      "'vel':[%i,%i,%i,%i,%i],"
                                      "'inc':%i,"
                                      "'ori':%i,"
                                      "'tem':%i,"
                                      "'hum':%i,"
                                      "'uls':%i,"
                                      "'lls':%i"
                                      "}\"",
                       1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 24, 15, 25, 78, 0, 1);
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
                                      "'dis':[%i,%i,%i,%i,%i,%i,%i,%i,%i],"
                                      "'mot':[%i,%i,%i,%i,%i],"
                                      "'inc':%i,"
                                      "'tem':%i,"
                                      "'hum':%i,"
                                      "'uls':%i,"
                                      "'lls':%i"
                                      "}\"",
                       1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1);
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
  DEBUG_INIT();
  DEBUG_PRINTLN(F("SETUP IN"));
  delay(5000);
  screen_init();
  interval_init();
  // load_init();
  // distance_init();
  // temphumi_init();
  wheel_init();
  wheel_set_MaxSpeed(100);
  wheel_turn_right(0.5);
  delay(5000);
  wheel_stop();
  // ptz_init();

  ros_init(SUB_MsgToSauron_app_cb, PUB_Sensor_app_cb, PUB_AlertSauron_app_cb);

  DEBUG_PRINTLN(F("SETUP OUT"));
}

void loop()
{

  if (interval_100ms_triggered())
  {
    ros_loop();
  }

  if (interval_500ms_triggered())
  {
  }
  if (interval_2000ms_triggered())
  {
  }

  if (interval_5000ms_triggered())
  {
  }
}