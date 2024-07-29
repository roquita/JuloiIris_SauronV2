#include "main.h"

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
                                      "'lls':%i,"
                                      "'vmx':%i"
                                      "}\"",
                       distance_get_Front_mm() / 10, distance_get_RightFront_mm() / 10,
                       distance_get_RightMiddle_mm() / 10, distance_get_RightBack_mm() / 10,
                       distance_get_LeftFront_mm() / 10, distance_get_LeftMiddle_mm() / 10,
                       distance_get_LeftBack_mm() / 10, distance_get_BackRight_mm() / 10,
                       distance_get_BackLeft_mm() / 10,
                       0, wheel_FrontLeft_get_speed(), 0, 0, 0,
                       0, 0,
                       (int)temphumi_get_temperature(), (int)temphumi_get_humidity(),
                       ptz_uls_pressed() == true ? 1 : 0, ptz_lls_pressed() == true ? 1 : 0,
                       wheel_get_MaxSpeed());
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