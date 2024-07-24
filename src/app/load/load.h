#pragma once

typedef enum
{
    LOAD_STATUS_ON,
    LOAD_STATUS_OFF,
    LOAD_STATUS_TOOGLING,
    LOAD_STATUS_TOOGLING_1s_5s,
} load_status_t;

void load_init();

void load_TurnOn_circulin();
void load_TurnOff_circulin();
void load_toogling_circulin();

void load_TurnOn_PilotRed();
void load_TurnOff_PilotRed();
void load_toogling_PilotRed();
void load_Toogling_1s_5s_PilotRed();

void load_TurnOn_PilotYellow();
void load_TurnOff_PilotYellow();
void load_toogling_PilotYellow();

void load_TurnOn_led();
void load_TurnOff_led();
void load_toogling_led();

void load_TurnOn_light();
void load_TurnOff_light();


