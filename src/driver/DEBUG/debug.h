#include "Arduino.h"

#define DEBUG_ENABLED (true)

#if (DEBUG_ENABLED == true)
#define DEBUG_INIT() Serial3.begin(2400)
#define DEBUG_PRINT(format) Serial3.print(format)
#define DEBUG_PRINTLN(format) Serial3.println(format)
#define DEBUG_GET_NUM(pnum)                                         \
    {                                                               \
        if (Serial3.available() > 0)                                \
        {                                                           \
            uint8_t buffer[4] = {0};                                \
            Serial3.readBytes(buffer, 3);                           \
            buffer[0] -= '0';                                       \
            buffer[1] -= '0';                                       \
            buffer[2] -= '0';                                       \
            int num = buffer[0] * 100 + buffer[1] * 10 + buffer[2]; \
            *pnum = num;                                            \
            Serial3.print(F("Received:"));                          \
            Serial3.println(num);                                   \
        }                                                           \
    }
#else
#define DEBUG_INIT()
#define DEBUG_PRINT (format, ...)
#define DEBUG_PRINTLN (format, ...)
#endif

/*
            Serial3.println(buffer[1]);                             \
            Serial3.println(buffer[2]);                             \

*/