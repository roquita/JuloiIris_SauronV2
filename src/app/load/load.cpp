#include "Arduino.h"
#include "../../driver/DEBUG/debug.h"
#include "load.h"
#include "project_defines.h"

static IntervalTimer timer;

load_status_t _circulin_status = LOAD_STATUS_OFF;
load_status_t _PilotRed_status = LOAD_STATUS_OFF;
load_status_t _PilotYellow_status = LOAD_STATUS_OFF;
load_status_t _led_status = LOAD_STATUS_OFF;
load_status_t _light_status = LOAD_STATUS_OFF;

int _circulin_counter = 0;
int _PilotRed_counter = 0;
int _PilotYellow_counter = 0;
int _led_counter = 0;

int _circulin_pulses = 0;
int _PilotRed_pulses = 0;
int _PilotYellow_pulses = 0;
int _led_pulses = 0;

void digitalToogle_xs_5s(int pin, int x, int *pcounter, int *ppulses)
{
    int counter = *pcounter;
    int pulses = *ppulses;

    counter++;

    if ((digitalRead(pin) == HIGH) && (counter >= 2))
    {
        digitalWrite(pin, LOW);
        counter = 0;
        pulses++;
    }
    if ((pulses >= x) && (digitalRead(pin) == LOW) && (counter >= 10))
    {
        digitalWrite(pin, HIGH);
        counter = 0;
        pulses = 0;
    }
    if ((pulses < x) && (digitalRead(pin) == LOW) && (counter >= 2))
    {
        digitalWrite(pin, HIGH);
        counter = 0;
    }

    *pcounter = counter;
    *ppulses = pulses;
}

void timer_cb()
{
    // circulin
    switch (_circulin_status)
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

    // pilot Red
    switch (_PilotRed_status)
    {
    case LOAD_STATUS_ON:
        digitalWrite(RELAY_3_PIN, 1);
        break;
    case LOAD_STATUS_OFF:
        digitalWrite(RELAY_3_PIN, 0);
        break;
    case LOAD_STATUS_TOOGLING:
        digitalToggle(RELAY_3_PIN);
        break;
    default:
        digitalToogle_xs_5s(RELAY_3_PIN, 1, &_PilotRed_counter, &_PilotRed_pulses);
        break;
    }

    // pilot Yellow
    switch (_PilotYellow_status)
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
    switch (_led_status)
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

    // light
    switch (_light_status)
    {
    case LOAD_STATUS_ON:
        digitalWrite(RELAY_2_PIN, HIGH);
        digitalWrite(RELAY_3_PIN, HIGH);
        digitalWrite(RELAY_4_PIN, HIGH);
        break;
    case LOAD_STATUS_OFF:
        digitalWrite(RELAY_2_PIN, LOW);
        digitalWrite(RELAY_3_PIN, LOW);
        digitalWrite(RELAY_4_PIN, LOW);
        break;
    default:
        digitalToggle(RELAY_2_PIN);
        digitalToggle(RELAY_3_PIN);
        digitalToggle(RELAY_4_PIN);

        break;
    }
}

void load_init()
{
    // circulin
    // pinMode(RELAY_1_PIN, OUTPUT); // en
    // pinMode(RELAY_2_PIN, OUTPUT); // sta

    // pilot 1
    // pinMode(RELAY_3_PIN, OUTPUT);

    // pilot 2
    // pinMode(RELAY_4_PIN, OUTPUT);

    // led
    pinMode(BUILTIN_LED_PIN, OUTPUT);

    // light
    pinMode(RELAY_2_PIN, OUTPUT);
    pinMode(RELAY_3_PIN, OUTPUT);
    pinMode(RELAY_4_PIN, OUTPUT);

    timer.begin(timer_cb, 500000UL);
}
void load_TurnOn_circulin()
{
    _circulin_status = LOAD_STATUS_ON;
}
void load_TurnOff_circulin()
{
    _circulin_status = LOAD_STATUS_OFF;
}
void load_toogling_circulin()
{
    _circulin_status = LOAD_STATUS_TOOGLING;
}
void load_TurnOn_pilotRed()
{
    _PilotRed_status = LOAD_STATUS_ON;
}
void load_TurnOff_PilotRed()
{
    _PilotRed_status = LOAD_STATUS_OFF;
}
void load_toogling_PilotRed()
{
    _PilotRed_status = LOAD_STATUS_TOOGLING;
}
void load_Toogling_1s_5s_PilotRed()
{
    _PilotRed_status = LOAD_STATUS_TOOGLING_1s_5s;
}
void load_TurnOn_PilotYellow()
{
    _PilotYellow_status = LOAD_STATUS_ON;
}
void load_TurnOff_PilotYellow()
{
    _PilotYellow_status = LOAD_STATUS_OFF;
}
void load_toogling_PilotYellow()
{
    _PilotYellow_status = LOAD_STATUS_TOOGLING;
}

void load_TurnOn_led()
{
    _led_status = LOAD_STATUS_ON;
}
void load_TurnOff_led()
{
    _led_status = LOAD_STATUS_OFF;
}
void load_toogling_led()
{
    _led_status = LOAD_STATUS_TOOGLING;
}

void load_TurnOn_light()
{
    _light_status = LOAD_STATUS_ON;
}
void load_TurnOff_light()
{
    _light_status = LOAD_STATUS_OFF;
}