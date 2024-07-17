
#include <Arduino.h>
#include "app/load/load.h"
#include "app/screen/screen.h"
#include "app/sensor/distance.h"
#include "app/sensor/temphumi.h"

#include "project_defines.h"

#include "DFRobot_QMC5883.h"

DFRobot_QMC5883 compass(&Wire2, 0x0D);

void setup()
{
  Serial.begin(115200);

  screen_init();
  screen_goto_MainPage();

  distance_init();
  temphumi_init();
  load_init();
  load_toogling_led();

  while (!compass.begin())
  {
    Serial.println("Could not find a valid 5883 sensor, check wiring!");
    delay(500);
  }
  if (compass.isQMC())
  {
    Serial.println("Initialize QMC5883");
     compass.setRange(QMC5883_RANGE_8GA);
     Serial.print("compass range is:");
     Serial.println(compass.getRange());

     compass.setMeasurementMode(QMC5883_CONTINOUS);
     Serial.print("compass measurement mode is:");
     Serial.println(compass.getMeasurementMode());

     compass.setDataRate(QMC5883_DATARATE_10HZ);
     Serial.print("compass data rate is:");
     Serial.println(compass.getDataRate());

     compass.setSamples(QMC5883_SAMPLES_8);
     Serial.print("compass samples is:");
     Serial.println(compass.getSamples());
  }
}
// int i = 0;

void loop()
{

  //float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
  //compass.setDeclinationAngle(declinationAngle);
  sVector_t mag = compass.readRaw();
  //compass.getHeadingDegrees();
  Serial.print("X:");
  Serial.print(mag.XAxis);
  Serial.print(" Y:");
  Serial.print(mag.YAxis);
  Serial.print(" Z:");
  Serial.println(mag.ZAxis);
 // Serial.print("Degress = ");
  //Serial.println(mag.HeadingDegress);
  delay(1000);



}
