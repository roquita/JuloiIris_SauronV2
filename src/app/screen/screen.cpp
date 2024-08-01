
#include "Arduino.h"
#include "../../driver/NEXTION/nextion.h"
#include "../../driver/DEBUG/debug.h"
#include "screen.h"

static char buffer[100];

screen_notification_t notification1;
screen_notification_t notification2;
screen_notification_t notification3;
screen_notification_t notification4;
screen_notification_t notification5;
screen_notification_t notification6;

void screen_init()
{
    nextion_init();

    screen_goto_MainPage();

    // SENSORS

    screen_print_DistanceFront(0.0);
    screen_print_DistanceRightFront(0.0);
    screen_print_DistanceRightMiddle(0.0);
    screen_print_DistanceRightBack(0.0);
    screen_print_DistanceLeftFront(0.0);
    screen_print_DistanceLeftMiddle(0.0);
    screen_print_DistanceLeftBack(0.0);
    screen_print_DistanceBackRight(0.0);
    screen_print_DistanceBackLeft(0.0);

    screen_print_BatteryLevel(0.0);
    screen_print_EnvironmentTemperature(0.0);
    screen_print_EnvironmentHumidity(0.0);

    // NOTIFICATIONS

    screen_notification_t empty_notification;
    empty_notification.color = SCREEN_COLOR_WHITE;
    empty_notification.content[0] = 0;
    empty_notification.footer = "";

    memcpy(&notification1, &empty_notification, sizeof(screen_notification_t));
    memcpy(&notification2, &empty_notification, sizeof(screen_notification_t));
    memcpy(&notification3, &empty_notification, sizeof(screen_notification_t));
    memcpy(&notification4, &empty_notification, sizeof(screen_notification_t));
    memcpy(&notification5, &empty_notification, sizeof(screen_notification_t));
    memcpy(&notification6, &empty_notification, sizeof(screen_notification_t));

    screen_print_Notification1();
    screen_print_Notification2();
    screen_print_Notification3();
    screen_print_Notification4();
    screen_print_Notification5();
    screen_print_Notification6();
}
void screen_print_DistanceFront(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_DistanceFront"));
        return;
    }

    nextion_select();
    nextion_set_txt("t0", buffer);
}
void screen_print_DistanceRightFront(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_RightFront"));
        return;
    }

    nextion_select();
    nextion_set_txt("t2", buffer);
}
void screen_print_DistanceRightMiddle(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_RightMiddle"));
        return;
    }

    nextion_select();
    nextion_set_txt("t3", buffer);
}
void screen_print_DistanceRightBack(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_RightBack"));
        return;
    }

    nextion_select();
    nextion_set_txt("t5", buffer);
}
void screen_print_DistanceLeftFront(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_LeftFront"));
        return;
    }

    nextion_select();
    nextion_set_txt("t1", buffer);
}
void screen_print_DistanceLeftMiddle(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_LeftMiddle"));
        return;
    }

    nextion_select();
    nextion_set_txt("t4", buffer);
}
void screen_print_DistanceLeftBack(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_LeftBack"));
        return;
    }

    nextion_select();
    nextion_set_txt("t8", buffer);
}
void screen_print_DistanceBackRight(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_BackRight"));
        return;
    }

    nextion_select();
    nextion_set_txt("t6", buffer);
}
void screen_print_DistanceBackLeft(int cm)
{
    int size = snprintf(buffer, sizeof(buffer), "%icm", cm);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_BackLeft"));
        return;
    }

    nextion_select();
    nextion_set_txt("t7", buffer);
}

void screen_print_BatteryLevel(int percent)
{
    int size = snprintf(buffer, sizeof(buffer), "%i", percent);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_BatteryLevel"));
        return;
    }

    nextion_select();
    nextion_set_txt("t26", buffer);
}
void screen_print_EnvironmentTemperature(int celsius)
{
    int size = snprintf(buffer, sizeof(buffer), "%i", celsius);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_EnvironmentTemperature"));
        return;
    }

    nextion_select();
    nextion_set_txt("t27", buffer);
}
void screen_print_EnvironmentHumidity(int percent)
{
    int size = snprintf(buffer, sizeof(buffer), "%i", percent);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("screen_print_EnvironmentHumidity"));
        return;
    }

    nextion_select();
    nextion_set_txt("t28", buffer);
}

void screen_print_Notification1()
{
    // BACKGROUND COLOR
    int pic;
    if (notification1.color == SCREEN_COLOR_WHITE)
        pic = 7;
    else if (notification1.color == SCREEN_COLOR_RED)
        pic = 4;
    else if (notification1.color == SCREEN_COLOR_YELLOW)
        pic = 3;
    else
        pic = 2;

    nextion_select();

    nextion_set_pic("p2", pic);

    // CONTENT TXT
    nextion_set_txt("t15", notification1.content);
    nextion_set_bco("t15", notification1.color);

    // FOOTER TXT
    nextion_set_txt("t14", notification1.footer);
    nextion_set_bco("t14", notification1.color);
}
void screen_print_Notification2()
{
    // BACKGROUND COLOR
    int pic;
    if (notification2.color == SCREEN_COLOR_WHITE)
        pic = 7;
    else if (notification2.color == SCREEN_COLOR_RED)
        pic = 4;
    else if (notification2.color == SCREEN_COLOR_YELLOW)
        pic = 3;
    else
        pic = 2;

    nextion_select();

    nextion_set_pic("p3", pic);

    // CONTENT TXT
    nextion_set_txt("t17", notification2.content);
    nextion_set_bco("t17", notification2.color);

    // FOOTER TXT
    nextion_set_txt("t16", notification2.footer);
    nextion_set_bco("t16", notification2.color);
}
void screen_print_Notification3()
{
    // BACKGROUND COLOR
    int pic;
    if (notification3.color == SCREEN_COLOR_WHITE)
        pic = 7;
    else if (notification3.color == SCREEN_COLOR_RED)
        pic = 4;
    else if (notification3.color == SCREEN_COLOR_YELLOW)
        pic = 3;
    else
        pic = 2;

    nextion_select();

    nextion_set_pic("p4", pic);

    // CONTENT TXT
    nextion_set_txt("t19", notification3.content);
    nextion_set_bco("t19", notification3.color);

    // FOOTER TXT
    nextion_set_txt("t18", notification3.footer);
    nextion_set_bco("t18", notification3.color);
}
void screen_print_Notification4()
{
    // BACKGROUND COLOR
    int pic;
    if (notification4.color == SCREEN_COLOR_WHITE)
        pic = 7;
    else if (notification4.color == SCREEN_COLOR_RED)
        pic = 4;
    else if (notification4.color == SCREEN_COLOR_YELLOW)
        pic = 3;
    else
        pic = 2;

    nextion_select();

    nextion_set_pic("p5", pic);

    // CONTENT TXT
    nextion_set_txt("t21", notification4.content);
    nextion_set_bco("t21", notification4.color);

    // FOOTER TXT
    nextion_set_txt("t20", notification4.footer);
    nextion_set_bco("t20", notification4.color);
}
void screen_print_Notification5()
{
    // BACKGROUND COLOR
    int pic;
    if (notification5.color == SCREEN_COLOR_WHITE)
        pic = 7;
    else if (notification5.color == SCREEN_COLOR_RED)
        pic = 4;
    else if (notification5.color == SCREEN_COLOR_YELLOW)
        pic = 3;
    else
        pic = 2;

    nextion_select();

    nextion_set_pic("p6", pic);

    // CONTENT TXT
    nextion_set_txt("t23", notification5.content);
    nextion_set_bco("t23", notification5.color);

    // FOOTER TXT
    nextion_set_txt("t22", notification5.footer);
    nextion_set_bco("t22", notification5.color);
}
void screen_print_Notification6()
{
    // BACKGROUND COLOR
    int pic;
    if (notification6.color == SCREEN_COLOR_WHITE)
        pic = 7;
    else if (notification6.color == SCREEN_COLOR_RED)
        pic = 4;
    else if (notification6.color == SCREEN_COLOR_YELLOW)
        pic = 3;
    else
        pic = 2;

    nextion_select();

    nextion_set_pic("p7", pic);

    // CONTENT TXT
    nextion_set_txt("t25", notification6.content);
    nextion_set_bco("t25", notification6.color);

    // FOOTER TXT
    nextion_set_txt("t24", notification6.footer);
    nextion_set_bco("t24", notification6.color);
}

void screen_queue_Notification(screen_notification_t *notification)
{
    memcpy(&notification6, &notification5, sizeof(screen_notification_t));
    screen_print_Notification6();

    memcpy(&notification5, &notification4, sizeof(screen_notification_t));
    screen_print_Notification5();

    memcpy(&notification4, &notification3, sizeof(screen_notification_t));
    screen_print_Notification4();

    memcpy(&notification3, &notification2, sizeof(screen_notification_t));
    screen_print_Notification3();

    memcpy(&notification2, &notification1, sizeof(screen_notification_t));
    screen_print_Notification2();

    memcpy(&notification1, notification, sizeof(screen_notification_t));
    screen_print_Notification1();
}
void screen_queue_NoInternet_Notification()
{
    screen_notification_t notification;
    notification.color = SCREEN_COLOR_RED;
    notification.content = "No Internet";
    notification.footer = "Fallo !!";
    screen_queue_Notification(&notification);
}
void screen_queue_YesInternet_Notificacion()
{
    screen_notification_t notification;
    notification.color = SCREEN_COLOR_GREEN;
    notification.content = "Internet Ready";
    notification.footer = "Exitoso !!";
    screen_queue_Notification(&notification);
}
void screen_queue_LowBatery_Notificacion()
{
    screen_notification_t notification;
    notification.color = SCREEN_COLOR_YELLOW;
    notification.content = "Bateria baja";
    notification.footer = "Aviso !!";
    screen_queue_Notification(&notification);
}
void screen_queue_SetupDone_Notificacion()
{
    screen_notification_t notification;
    notification.color = SCREEN_COLOR_GREEN;
    notification.content = "Setup Done";
    notification.footer = "Exitoso !!";
    screen_queue_Notification(&notification);
}
void screen_goto_MainPage()
{
    nextion_select();
    nextion_set_page(SCREEN_PAGE_MAIN);
}
void screen_goto_LoadingPage()
{
    nextion_select();
    nextion_set_page(SCREEN_PAGE_LOADING);
}
