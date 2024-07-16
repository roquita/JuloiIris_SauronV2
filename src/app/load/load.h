#pragma once

typedef enum
{
    LOAD_STATUS_ON,
    LOAD_STATUS_OFF,
    LOAD_STATUS_TOOGLING,
} load_status_t;

void load_init();

void load_TurnOn_circulin();
void load_TurnOff_circulin();
void load_toogling_circulin();

void load_TurnOn_pilot1();
void load_TurnOff_pilot1();
void load_toogling_pilot1();

void load_TurnOn_pilot2();
void load_TurnOff_pilot2();
void load_toogling_pilot2();

void load_TurnOn_led();
void load_TurnOff_led();
void load_toogling_led();