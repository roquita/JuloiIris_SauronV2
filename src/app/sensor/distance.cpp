#include "Arduino.h"
#include "../../driver/XKC-KL200/xkc_kl200.h"
#include "../../driver/DEBUG/debug.h"

void distance_init()
{
    xkc_kl200_init();
}
bool distance_get_Front(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_1);
    return xkc_kl200_get_data(mm);
}
bool distance_get_RightFront(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_2);
    return xkc_kl200_get_data(mm);
}
bool distance_get_RightMiddle(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_3);
    return xkc_kl200_get_data(mm);
}
bool distance_get_RightBack(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_4);
    return xkc_kl200_get_data(mm);
}
bool distance_get_LeftFront(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_5);
    return xkc_kl200_get_data(mm);
}
bool distance_get_LeftMiddle(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_6);
    return xkc_kl200_get_data(mm);
}
bool distance_get_LeftBack(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_7);
    return xkc_kl200_get_data(mm);
}
bool distance_get_BackRight(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_8);
    return xkc_kl200_get_data(mm);
}
bool distance_get_BackLeft(int *mm)
{
    xkc_kl200_select(XKC_KL200_DEVICE_9);
    return xkc_kl200_get_data(mm);
}