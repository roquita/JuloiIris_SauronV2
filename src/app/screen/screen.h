#pragma once

#define SCREEN_COLOR_WHITE 65535
#define SCREEN_COLOR_RED 53699
#define SCREEN_COLOR_YELLOW 64832
#define SCREEN_COLOR_GREEN 5728

typedef struct
{
    int color;
    char content[10];
    char* footer;
} screen_notification_t;

typedef enum
{
    SCREEN_PAGE_LOADING = 1,
    SCREEN_PAGE_MAIN = 0,
} screen_page_t;

void screen_init();
void screen_print_DistanceFront(float value);
void screen_print_DistanceRightFront(float value);
void screen_print_DistanceRightMiddle(float value);
void screen_print_DistanceRightBack(float value);
void screen_print_DistanceLeftFront(float value);
void screen_print_DistanceLeftMiddle(float value);
void screen_print_DistanceLeftBack(float value);
void screen_print_DistanceBackRight(float value);
void screen_print_DistanceBackLeft(float value);

void screen_print_BatteryLevel(float value);
void screen_print_EnvironmentTemperature(float value);
void screen_print_EnvironmentHumidity(float value);

void screen_print_Notification1();
void screen_print_Notification2();
void screen_print_Notification3();
void screen_print_Notification4();
void screen_print_Notification5();
void screen_print_Notification6();
void screen_queue_Notification(screen_notification_t *notification);

void screen_goto_MainPage();
void screen_goto_LoadingPage();