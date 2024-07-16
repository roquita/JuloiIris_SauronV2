#include "Arduino.h"
#include "../../driver/DEBUG/debug.h"
#include <SimpleDHT.h>
#include "project_defines.h"

SimpleDHT11 dht11(DHT11_PIN);
static float temperature = 0.0;
static float humidity = 0.0;

void temphumi_init() {}
void temphumi_update()
{
    bool reading_success = dht11.read2(&temperature, &humidity, NULL) == SimpleDHTErrSuccess;
    if (!reading_success)
    {
        DEBUG_PRINTLN(F("temphumi_get_reading 01"));
        return;
    }

    return;
}
float temphumi_get_temperature()
{
    return temperature;
}
float temphumi_get_humidity()
{
    return humidity;
}