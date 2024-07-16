#pragma once

// PINOUT 
#define RELAY_1_PIN 23 // circulin red
#define RELAY_2_PIN 22 // circulin yellow
#define RELAY_3_PIN 21 // pilot
#define RELAY_4_PIN 20
#define RELAY_5_PIN 19
#define RELAY_6_PIN 18
#define RELAY_7_PIN 17
#define RELAY_8_PIN 16

#define PTZ_TX_PIN 15 // uart 3
#define PTZ_RX_PIN 14 // uart 3

#define BUILTIN_LED_PIN 13

#define SWITCH_2_PIN 41
#define SWITCH_1_PIN 40

#define MUX_S3_PIN 39
#define MUX_S2_PIN 38
#define MUX_S1_PIN 37
#define MUX_S0_PIN 36
#define MUX_RX_PIN 35 // uart 8
#define MUX_TX_PIN 34 // uart 8

#define DHT11_PIN 33

#define I2C_SDA_2_PIN 25 // i2c 2
#define I2C_SCL_2_PIN 24 // i2c 2

#define DRIVER_1_ALARM_PIN 32
#define DRIVER_1_SPEED_PIN 31
#define DRIVER_1_FR_PIN 30
#define DRIVER_1_SV_PIN 29

#define DRIVER_2_ALARM_PIN 28
#define DRIVER_2_SPEED_PIN 27
#define DRIVER_2_FR_PIN 26
#define DRIVER_2_SV_PIN 12

#define DRIVER_3_ALARM_PIN 11
#define DRIVER_3_SPEED_PIN 10
#define DRIVER_3_FR_PIN 9
#define DRIVER_3_SV_PIN 8

#define DRIVER_4_ALARM_PIN 7
#define DRIVER_4_SPEED_PIN 6
#define DRIVER_4_FR_PIN 5
#define DRIVER_4_SV_PIN 4

#define DRIVER_5_ALARM_PIN 3
#define DRIVER_5_SPEED_PIN 2
#define DRIVER_5_FR_PIN 1
#define DRIVER_5_SV_PIN 0

// I2C ADDRESS
#define MAG_ADDRESS 0x0D
#define ACC_GYR_ADDRESS 0x6A

// motor driver for PTZ
#define USE_DRIVER_V2 1