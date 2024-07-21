#include "Arduino.h"
#include "interval.h"

static int time_100ms = 0;
static int time_500ms = 0;
static int time_1000ms = 0;
static int time_2000ms = 0;
static int time_5000ms = 0;

void interval_init()
{
    time_100ms = millis();
    time_500ms = millis();
    time_1000ms = millis();
    time_2000ms = millis();
    time_5000ms = millis();
}
bool interval_100ms_triggered()
{
    if (millis() - time_100ms > 100)
    {
        time_100ms = millis();
        return true;
    }
    return false;
}
bool interval_500ms_triggered()
{
    if (millis() - time_500ms > 500)
    {
        time_500ms = millis();
        return true;
    }
    return false;
}
bool interval_1000ms_triggered()
{
    if (millis() - time_1000ms > 1000)
    {
        time_1000ms = millis();
        return true;
    }
    return false;
}
bool interval_2000ms_triggered()
{
    if (millis() - time_2000ms > 2000)
    {
        time_2000ms = millis();
        return true;
    }
    return false;
}
bool interval_5000ms_triggered()
{
    if (millis() - time_5000ms > 5000)
    {
        time_5000ms = millis();
        return true;
    }
    return false;
}