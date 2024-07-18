#include "Arduino.h"

#define DEBUG_ENABLED (true)

#if (DEBUG_ENABLED == true)
#define DEBUG_INIT() Serial3.begin(9600)
#define DEBUG_PRINT(format) Serial3.print(format)
#define DEBUG_PRINTLN(format) Serial3.println(format)
#else
#define DEBUG_INIT()
#define DEBUG_PRINT (format, ...)
#define DEBUG_PRINTLN (format, ...)
#endif