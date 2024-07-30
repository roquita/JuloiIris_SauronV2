#pragma once

typedef void (*Ros_at_transport_disconnected_t)(void);
typedef void (*Ros_at_transport_conencted_t)();
typedef void (*Locomotion_app_cb_t)(int direction, float velocity);
typedef void (*MaxVelocity_app_cb_t)(int MaxVelocity);
typedef void (*Tower_app_cb_t)(int method, int value);
typedef void (*Alert_app_cb_t)(int AlertCode);
typedef void (*Load_app_cb_t)(int LoadCode);
typedef void (*Battery_app_cb_t)(int BatteryLevel);

typedef bool (*Sensor_app_cb_t)(char *buffer, int *size);
typedef bool (*AlertSauron_app_cb_t)(char *buffer, int *size);

void ros_init(Ros_at_transport_disconnected_t Ros_at_transport_disconnected,
              Ros_at_transport_conencted_t Ros_at_transport_conencted,
              Locomotion_app_cb_t Locomotion_app_cb,
              MaxVelocity_app_cb_t MaxVelocity_app_cb,
              Tower_app_cb_t Tower_app_cb,
              Alert_app_cb_t Alert_app_cb,
              Load_app_cb_t Load_app_cb,
              Battery_app_cb_t Battery_app_cb,
              Sensor_app_cb_t Sensor_app_cb,
              AlertSauron_app_cb_t AlertSauron_app_cb);
void ros_loop();
