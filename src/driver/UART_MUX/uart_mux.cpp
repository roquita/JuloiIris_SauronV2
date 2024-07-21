#include "Arduino.h"
#include "uart_mux.h"
#include "project_defines.h"
#include "../DEBUG/debug.h"

static bool initialized = false;

void UARTMUX_init()
{
    if (initialized == true)
    {
        DEBUG_PRINTLN(F("UART MUX already initialized"));
        return;
    }
    initialized = true;

    Serial8.begin(2400);
    Serial8.setTimeout(1000);
    
    pinMode(MUX_S3_PIN, OUTPUT);
    pinMode(MUX_S2_PIN, OUTPUT);
    pinMode(MUX_S1_PIN, OUTPUT);
    pinMode(MUX_S0_PIN, OUTPUT);

    UARTMUX_select_channel(MUX_CHANNEL_0);
}
void UARTMUX_print(char *str)
{
    Serial8.print(str);
}
void UARTMUX_write(int byte)
{
    Serial8.write(byte);
}
bool UARTMUX_read(uint8_t *data, int n)
{
    int bytes = Serial8.readBytes(data, n);
    return bytes == n;
}
void UARTMUX_select_channel(mux_channel_t channel)
{
    switch (channel)
    {
    case MUX_CHANNEL_0:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_1:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_2:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_3:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_4:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_5:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_6:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_7:
        digitalWrite(MUX_S3_PIN, 0);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_8:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_9:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_10:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_11:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 0);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_12:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    case MUX_CHANNEL_13:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 0);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    case MUX_CHANNEL_14:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 0);
        break;
    default:
        digitalWrite(MUX_S3_PIN, 1);
        digitalWrite(MUX_S2_PIN, 1);
        digitalWrite(MUX_S1_PIN, 1);
        digitalWrite(MUX_S0_PIN, 1);
        break;
    }
}