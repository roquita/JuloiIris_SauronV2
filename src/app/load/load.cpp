#include "Arduino.h"
#include "../../driver/DEBUG/debug.h"
#include "load.h"
#include "project_defines.h"

static IntervalTimer timer;

load_status_t circulin_status = LOAD_STATUS_OFF;
load_status_t pilot1_status = LOAD_STATUS_OFF;
load_status_t pilot2_status = LOAD_STATUS_OFF;
load_status_t led_status = LOAD_STATUS_OFF;

// int counter = 0;
void timer_cb()
{
    // circulin
    switch (circulin_status)
    {
    case LOAD_STATUS_ON:
        digitalWrite(RELAY_1_PIN, 1);
        digitalWrite(RELAY_2_PIN, 1);
        break;
    case LOAD_STATUS_OFF:
        digitalWrite(RELAY_1_PIN, 0);
        digitalWrite(RELAY_2_PIN, 0);
        break;
    default:
        digitalWrite(RELAY_1_PIN, 1);
        digitalToggle(RELAY_2_PIN);
        break;
    }

    // pilot 1
    switch (pilot1_status)
    {
    case LOAD_STATUS_ON:
        digitalWrite(RELAY_3_PIN, 1);
        break;
    case LOAD_STATUS_OFF:
        digitalWrite(RELAY_3_PIN, 0);
        break;
    default:
        digitalToggle(RELAY_3_PIN);
        break;
    }

    // pilot 2
    switch (pilot2_status)
    {
    case LOAD_STATUS_ON:
        digitalWrite(RELAY_4_PIN, 1);
        break;
    case LOAD_STATUS_OFF:
        digitalWrite(RELAY_4_PIN, 0);
        break;
    default:
        digitalToggle(RELAY_4_PIN);
        break;
    }

    // led
    switch (led_status)
    {
    case LOAD_STATUS_ON:
        digitalWrite(BUILTIN_LED_PIN, 1);
        break;
    case LOAD_STATUS_OFF:
        digitalWrite(BUILTIN_LED_PIN, 0);
        break;
    default:
        digitalToggle(BUILTIN_LED_PIN);
        break;
    }
}

void load_init()
{
    // circulin
    pinMode(RELAY_1_PIN, OUTPUT); // en
    pinMode(RELAY_2_PIN, OUTPUT); // sta

    // pilot 1
    pinMode(RELAY_3_PIN, OUTPUT);

    // pilot 2
    pinMode(RELAY_4_PIN, OUTPUT);

    // led
    pinMode(BUILTIN_LED_PIN, OUTPUT);

    timer.begin(timer_cb, 500000UL);
}
void load_TurnOn_circulin()
{
    circulin_status = LOAD_STATUS_ON;
}
void load_TurnOff_circulin()
{
    circulin_status = LOAD_STATUS_OFF;
}
void load_toogling_circulin()
{
    circulin_status = LOAD_STATUS_TOOGLING;
}
void load_TurnOn_pilot1()
{
    pilot1_status = LOAD_STATUS_ON;
}
void load_TurnOff_pilot1()
{
    pilot1_status = LOAD_STATUS_OFF;
}
void load_toogling_pilot1()
{
    pilot1_status = LOAD_STATUS_TOOGLING;
}
void load_TurnOn_pilot2()
{
    pilot2_status = LOAD_STATUS_ON;
}
void load_TurnOff_pilot2()
{
    pilot2_status = LOAD_STATUS_OFF;
}
void load_toogling_pilot2()
{
    pilot2_status = LOAD_STATUS_TOOGLING;
}

void load_TurnOn_led()
{
    led_status = LOAD_STATUS_ON;
}
void load_TurnOff_led()
{
    led_status = LOAD_STATUS_OFF;
}
void load_toogling_led()
{
    led_status = LOAD_STATUS_TOOGLING;
}