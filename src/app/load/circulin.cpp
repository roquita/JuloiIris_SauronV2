#include "Arduino.h"
#include "project_defines.h"

#define CIRCULIN_RED_PIN (RELAY_1_PIN)
#define CIRCULIN_YELLOW_PIN (RELAY_2_PIN)

void circulin_init()
{
    pinMode(CIRCULIN_RED_PIN, OUTPUT);
    pinMode(CIRCULIN_YELLOW_PIN, OUTPUT);
}
void circulin_yellow_on()
{
    digitalWrite(CIRCULIN_YELLOW_PIN, 1);
}
void circulin_yellow_off()
{
    digitalWrite(CIRCULIN_YELLOW_PIN, 0);
}
void circulin_red_on()
{
    digitalWrite(CIRCULIN_RED_PIN, 1);
}
void circulin_red_off()
{
    digitalWrite(CIRCULIN_RED_PIN, 0);
}