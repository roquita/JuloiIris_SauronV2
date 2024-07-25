#pragma once

void nextion_init();
void nextion_select();
void nextion_set_txt(char *obj, char *txt);
void nextion_add_txt(char *obj, char *txt);
void nextion_set_pic(char *obj, int pic);
void nextion_set_bco(char *obj, int bco);
void nextion_set_page(int page);