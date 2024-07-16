#pragma once

//62 30 09 FF FF 00 00 00 5b  baudrate 2400
//62 34 09 FF FF 00 01 00 5f  manual mode
//62 33 09 FF FF 00 00 00 58  request data
//62 33 09 00 00 00 6D 66 53  answer for request data

typedef enum{
    XKC_KL200_DEVICE_1,
    XKC_KL200_DEVICE_2,
    XKC_KL200_DEVICE_3,
    XKC_KL200_DEVICE_4,
    XKC_KL200_DEVICE_5,
    XKC_KL200_DEVICE_6,
    XKC_KL200_DEVICE_7,
    XKC_KL200_DEVICE_8,
    XKC_KL200_DEVICE_9,
}xkc_kl200_device_t;

void xkc_kl200_init();
void xkc_kl200_select(xkc_kl200_device_t device);
bool xkc_kl200_get_data(int *data);