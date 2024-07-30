#pragma once

typedef enum
{
    MUX_CHANNEL_0,
    MUX_CHANNEL_1,
    MUX_CHANNEL_2,
    MUX_CHANNEL_3,
    MUX_CHANNEL_4,
    MUX_CHANNEL_5,
    MUX_CHANNEL_6,
    MUX_CHANNEL_7,
    MUX_CHANNEL_8,
    MUX_CHANNEL_9,
    MUX_CHANNEL_10,
    MUX_CHANNEL_11,
    MUX_CHANNEL_12,
    MUX_CHANNEL_13,
    MUX_CHANNEL_14,
    MUX_CHANNEL_15,
} mux_channel_t;

void UARTMUX_init();
void UARTMUX_print(char *str);
void UARTMUX_write(int byte);
bool UARTMUX_read(uint8_t *data, int n);
void UARTMUX_flush();
int UARTMUX_available();
void UARTMUX_select_channel(mux_channel_t channel);