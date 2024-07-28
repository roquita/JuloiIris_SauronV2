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