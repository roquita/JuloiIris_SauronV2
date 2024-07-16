#include "Arduino.h"
#include "../../driver/DEBUG/debug.h"
#include <SimpleDHT.h>
#include "project_defines.h"

SimpleDHT11 dht11(DHT11_PIN);
//static float temperature = 0.0;
//static float humidity = 0.0;

void temphumi_init() {}
bool temphumi_get_reading(float *temperature, float *humidity)
{
    bool reading_success = dht11.read2(temperature, humidity, NULL) == SimpleDHTErrSuccess;
    if (!reading_success)
    {
        DEBUG_PRINTLN(F("temphumi_get_reading 01"));
        return false;
    }

    return true;
}
