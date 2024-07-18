#pragma once

typedef void (*suscriber_cb_t)(void);
typedef void (*publiser_cb_t)(void);

void ros_init(suscriber_cb_t suscriber01_cb, suscriber_cb_t suscriber02_cb, publiser_cb_t publisher01_cb, publiser_cb_t publisher02_cb);
void ros_loop();
