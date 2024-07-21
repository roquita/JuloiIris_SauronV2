#pragma once

typedef void (*MsgToSauron_app_cb_t)(int type, float param1, float param2);
typedef bool (*Sensor_app_cb_t)(char *buffer, int *size);
typedef bool (*AlertSauron_app_cb_t)(char *buffer, int *size);

void ros_init(MsgToSauron_app_cb_t MsgToSauron_app_cb,
              Sensor_app_cb_t Sensor_app_cb,
              AlertSauron_app_cb_t AlertSauron_app_cb);
void ros_loop();
