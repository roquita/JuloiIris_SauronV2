#include "Arduino.h"
#include "xkc_kl200.h"
#include "../UART_MUX/uart_mux.h"
#include "../DEBUG/debug.h"

static char buffer[30];

uint8_t xor8(uint8_t *data, size_t length)
{
    uint8_t checksum = 0;

    for (size_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

void xkc_kl200_init()
{
    UARTMUX_init();
}
void xkc_kl200_select(xkc_kl200_device_t device)
{
    switch (device)
    {
    case XKC_KL200_DEVICE_1:
        UARTMUX_select_channel(MUX_CHANNEL_14);
        break;
    case XKC_KL200_DEVICE_2:
        UARTMUX_select_channel(MUX_CHANNEL_13);
        break;
    case XKC_KL200_DEVICE_3:
        UARTMUX_select_channel(MUX_CHANNEL_12);
        break;
    case XKC_KL200_DEVICE_4:
        UARTMUX_select_channel(MUX_CHANNEL_11);
        break;
    case XKC_KL200_DEVICE_5:
        UARTMUX_select_channel(MUX_CHANNEL_10);
        break;
    case XKC_KL200_DEVICE_6:
        UARTMUX_select_channel(MUX_CHANNEL_9);
        break;
    case XKC_KL200_DEVICE_7:
        UARTMUX_select_channel(MUX_CHANNEL_8);
        break;
    case XKC_KL200_DEVICE_8:
        UARTMUX_select_channel(MUX_CHANNEL_6);
        break;
    default:
        UARTMUX_select_channel(MUX_CHANNEL_7);
        break;
    }
}
// 62 33 09 FF FF 00 00 00 58  request data
// 62 33 09 00 00 00 6D 66 53  answer for request data
void xkc_kl200_request_data()
{   
    // clean transport buffer
    UARTMUX_flush();
    // clean local buffer
    memset(buffer, 0, sizeof(buffer));
    // request
    UARTMUX_write(0x62);
    UARTMUX_write(0x33);
    UARTMUX_write(0x09);
    UARTMUX_write(0xff);
    UARTMUX_write(0xff);
    UARTMUX_write(0x00);
    UARTMUX_write(0x00);
    UARTMUX_write(0x00);
    UARTMUX_write(0x58);
}
bool xkc_kl200_available()
{
    return (UARTMUX_available() >= 9);
}
bool xkc_kl200_read(int *data)
{
    bool timeout = UARTMUX_read((uint8_t *)buffer, 9) == false;
    if (timeout)
    {
        DEBUG_PRINTLN(F("xkc_kl200_get_data 01"));
        *data = 0;
        return false;
    }
    // detect xor8 failure
    uint8_t code = xor8((uint8_t *)buffer, 8);
    bool xor_valid = (code == buffer[8]);
    if (!xor_valid)
    {
        DEBUG_PRINTLN(F("xkc_kl200_get_data 02"));
        *data = 0;
        return false;
    }
    // get reading
    *data = (((uint16_t)buffer[5]) << 8) + buffer[6];
    return true;
}
bool xkc_kl200_get_data(int *data)
{
    // clean buffer
    memset(buffer, 0, sizeof(buffer));
    // request
    UARTMUX_write(0x62);
    UARTMUX_write(0x33);
    UARTMUX_write(0x09);
    UARTMUX_write(0xff);
    UARTMUX_write(0xff);
    UARTMUX_write(0x00);
    UARTMUX_write(0x00);
    UARTMUX_write(0x00);
    UARTMUX_write(0x58);

    // detect timeout
    bool timeout = UARTMUX_read((uint8_t *)buffer, 9) == false;
    if (timeout)
    {
        DEBUG_PRINTLN(F("xkc_kl200_get_data 01"));
        *data = 0;
        return false;
    }
    // detect xor8 failure
    uint8_t code = xor8((uint8_t *)buffer, 8);
    bool xor_valid = (code == buffer[8]);
    if (!xor_valid)
    {
        DEBUG_PRINTLN(F("xkc_kl200_get_data 02"));
        *data = 0;
        return false;
    }
    // get reading
    *data = (((uint16_t)buffer[5]) << 8) + buffer[6];
    return true;
}