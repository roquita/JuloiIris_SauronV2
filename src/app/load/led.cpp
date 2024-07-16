#include "Arduino.h"
#include "project_defines.h"

void led_init()
{
    pinMode(BUILTIN_LED_PIN, OUTPUT);
}
void led_on()
{
    digitalWrite(BUILTIN_LED_PIN, 1);
}
void led_off()
{
    digitalWrite(BUILTIN_LED_PIN, 0);
}