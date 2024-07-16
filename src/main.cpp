#include <Arduino.h>
#include "app/load/circulin.h"
#include "app/load/led.h"
#include "app/load/pilot.h"
#include "app/screen/screen.h"
#include "app/sensor/distance.h"

#include "project_defines.h"

void setup()
{
  Serial.begin(115200);
  // load
  // circulin_init();
  // led_init();
  // pilot_init();

  // ptz
  // ptz_init();
  /*
  pinMode(MUX_S3_PIN, OUTPUT);
  pinMode(MUX_S2_PIN, OUTPUT);
  pinMode(MUX_S1_PIN, OUTPUT);
  pinMode(MUX_S0_PIN, OUTPUT);

  digitalWrite(MUX_S3_PIN, 1);
  digitalWrite(MUX_S2_PIN, 1);
  digitalWrite(MUX_S1_PIN, 1);
  digitalWrite(MUX_S0_PIN, 1);
  Serial8.begin(2400);
  Serial8.print(F("page 0\xff\xff\xff"));
  */
  // Serial8.write(255);
  // Serial8.write(255);
  // Serial8.write(255);

  screen_init();
  screen_goto_MainPage();

  distance_init();

  // sensor
  // distance_init();
  // humidity_init();
  // inclination_init();
  // orientation_init();
  // temperature_init();

  // wheel
  // wheel_init();
  pinMode(13, OUTPUT);
}
int i = 0;
void loop()
{
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);

  int mm = 0;
  bool reading_success = distance_get_LeftMiddle(&mm);
  if (reading_success)
  {
    screen_print_DistanceLeftMiddle((float)mm);
    Serial.println(mm);
  }
  delay(1000);
  /*
    screen_notification_t notification;
    notification.color = SCREEN_COLOR_GREEN;
    snprintf(notification.content, 10, "noti %i",i);
    notification.footer = "EXITO !!";
    screen_queue_Notification(&notification);

    i++;

    delay(5000);
    */
}
/*

+#include <stdio.h>
#include <stdint.h>
uint8_t xor8(uint8_t* byte, int size)
{
    uint8_t hash = 0;

    while (size > 0)
    {
        hash = (hash + *byte) & 0xff;
        byte++;
        size--;
    }
    return (((hash ^ 0xff) + 1) & 0xff);
}

uint8_t buffer[8]={0x64, 0x31, 0x09, 0xFF, 0xFF, 0x00, 0x00, 0x00};
int main()
{
    uint8_t crc = xor8(buffer,8);
    printf("xor8: 0x%02X", crc);
    return 0;
}

*/