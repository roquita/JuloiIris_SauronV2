#include "Arduino.h"

#define DEBUG_ENABLED (true)

#if (DEBUG_ENABLED == true)
#define DEBUG_PRINT(format) Serial.print(format)
#define DEBUG_PRINTLN(format) Serial.println(format)
#else
#define DEBUG_PRINT (format, ...)
#define DEBUG_PRINTLN (format, ...)
#endif