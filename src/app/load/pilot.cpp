#include "Arduino.h"
#include "project_defines.h"

#define PILOT_PIN (RELAY_3_PIN)

void pilot_init()
{
    pinMode(PILOT_PIN, OUTPUT);
}
void pilot_on()
{
    digitalWrite(PILOT_PIN, 1);
}
void pilot_off()
{
    digitalWrite(PILOT_PIN, 0);
}