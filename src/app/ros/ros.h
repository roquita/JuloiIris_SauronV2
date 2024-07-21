#pragma once

typedef void (*Locomotion_app_cb_t)(int direction, int velocity);
typedef void (*MaxVelocity_app_cb_t)(uint32_t MaxVelocity);
typedef void (*Tower_app_cb_t)(int method, int value);
typedef void (*ErrorJetson_app_cb_t)(uint32_t ErrorCode);
typedef bool (*Sensor_app_cb_t)(char *buffer, int *size);
typedef bool (*AlertSauron_app_cb_t)(char *buffer, int *size);

void ros_init(Locomotion_app_cb_t Locomotion_app_cb,
              MaxVelocity_app_cb_t MaxVelocity_app_cb,
              Tower_app_cb_t Tower_app_cb,
              ErrorJetson_app_cb_t ErrorJetson_app_cb,
              Sensor_app_cb_t Sensor_app_cb,
              AlertSauron_app_cb_t AlertSauron_app_cb);
void ros_loop();
