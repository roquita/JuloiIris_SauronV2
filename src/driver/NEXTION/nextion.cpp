#include "Arduino.h"
#include "nextion.h"
#include "../UART_MUX/uart_mux.h"
#include "../DEBUG/debug.h"

static char buffer[30];

void nextion_init()
{
    UARTMUX_init();
}
void nextion_select()
{
    UARTMUX_select_channel(MUX_CHANNEL_15);
}
void nextion_set_txt(char *obj, char *txt)
{
    int size = snprintf(buffer, sizeof(buffer), "main.%s.txt=\"%s\"\xff\xff\xff", obj, txt);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("nextion_set_txt"));
        return;
    }
    UARTMUX_print(buffer);
}
void nextion_set_pic(char *obj, int pic)
{
    int size = snprintf(buffer, sizeof(buffer), "main.%s.pic=%i\xff\xff\xff", obj, pic);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("nextion_set_pic"));
        return;
    }
    UARTMUX_print(buffer);
}
void nextion_set_bco(char *obj, int bco)
{
    int size = snprintf(buffer, sizeof(buffer), "main.%s.bco=%i\xff\xff\xff", obj, bco);
    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("nextion_set_bco"));
        return;
    }
    UARTMUX_print(buffer);
}
void nextion_set_page(int page)
{
    int size = snprintf(buffer, sizeof(buffer), "page %i\xff\xff\xff", page);

    if (size < 0 || size >= sizeof(buffer))
    {
        DEBUG_PRINTLN(F("nextion_set_page"));
        return;
    }

    UARTMUX_print(buffer);
}