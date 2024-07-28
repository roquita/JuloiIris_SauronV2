#include "Arduino.h"
#include "../../driver/XKC-KL200/xkc_kl200.h"
#include "../../driver/DEBUG/debug.h"

static int Front_mm = 0;
static int RightFront_mm = 0;
static int RightMiddle_mm = 0;
static int RightBack_mm = 0;
static int LeftFront_mm = 0;
static int LeftMiddle_mm = 0;
static int LeftBack_mm = 0;
static int BackRight_mm = 0;
static int BackLeft_mm = 0;

void distance_init()
{
    xkc_kl200_init();
}
void distance_update()
{
    xkc_kl200_select(XKC_KL200_DEVICE_6);
    xkc_kl200_get_data(&Front_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_9);
    xkc_kl200_get_data(&RightFront_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_8);
    xkc_kl200_get_data(&RightMiddle_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_7);
    xkc_kl200_get_data(&RightBack_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_1);
    xkc_kl200_get_data(&LeftFront_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_2);
    xkc_kl200_get_data(&LeftMiddle_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_3);
    xkc_kl200_get_data(&LeftBack_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_4);
    xkc_kl200_get_data(&BackRight_mm);

    xkc_kl200_select(XKC_KL200_DEVICE_5);
    xkc_kl200_get_data(&BackLeft_mm);
}
int distance_get_Front_mm()
{
    return Front_mm;
}
int distance_get_RightFront_mm()
{
    return RightFront_mm;
}
int distance_get_RightMiddle_mm()
{
    return RightMiddle_mm;
}
int distance_get_RightBack_mm()
{
    return RightBack_mm;
}
int distance_get_LeftFront_mm()
{
    return LeftFront_mm;
}
int distance_get_LeftMiddle_mm()
{
    return LeftMiddle_mm;
}
int distance_get_LeftBack_mm()
{
    return LeftBack_mm;
}
int distance_get_BackRight_mm()
{
    return BackRight_mm;
}
int distance_get_BackLeft_mm()
{
    return BackLeft_mm;
}