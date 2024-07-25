#pragma once

#define SCREEN_COLOR_WHITE 65535
#define SCREEN_COLOR_RED 53699
#define SCREEN_COLOR_YELLOW 64832
#define SCREEN_COLOR_GREEN 5728

#define SCREEN_DEBUG(x)           \
    {                             \
        nextion_select();         \
        nextion_set_txt("t9", x); \
    }

typedef struct
{
    int color;
    char *content;
    char *footer;
} screen_notification_t;
// char content[10];
typedef enum
{
    SCREEN_PAGE_LOADING = 1,
    SCREEN_PAGE_MAIN = 0,
} screen_page_t;

void screen_init();
void screen_print_DistanceFront(int cm);
void screen_print_DistanceRightFront(int cm);
void screen_print_DistanceRightMiddle(int cm);
void screen_print_DistanceRightBack(int cm);
void screen_print_DistanceLeftFront(int cm);
void screen_print_DistanceLeftMiddle(int cm);
void screen_print_DistanceLeftBack(int cm);
void screen_print_DistanceBackRight(int cm);
void screen_print_DistanceBackLeft(int cm);

void screen_print_BatteryLevel(int percent);
void screen_print_EnvironmentTemperature(int celsius);
void screen_print_EnvironmentHumidity(int percent);

void screen_print_Notification1();
void screen_print_Notification2();
void screen_print_Notification3();
void screen_print_Notification4();
void screen_print_Notification5();
void screen_print_Notification6();
void screen_queue_Notification(screen_notification_t *notification);

void screen_queue_NoInternet_Notification();
void screen_queue_YesInternet_Notificacion();
void screen_queue_LowBatery_Notificacion();

void screen_goto_MainPage();
void screen_goto_LoadingPage();
