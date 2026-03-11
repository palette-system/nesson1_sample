#ifndef __Nesso_Menu_H__
#define __Nesso_Menu_H__

#include <M5GFX.h>
#include <M5Unified.h>

class NessoMenu {
  public:
    M5Canvas *_menu_canvas;
    LGFX_Button btn_top;
    LGFX_Button btn_bttom;
    char **_menu_list;
    int menu_select;
    int _menu_length;
    int _slide_direction = 0;
    int _slide_index = 1;
    int _prev_y = -1;
    int _touch_end = 0;
    int _slide_pixel = 30;
    int _prev_touch_state;
    int clicked = -1;

    NessoMenu();
    void set_menu(char *menu_list_prm[], int menu_length_prm);
    void menu_loop();

};


#endif
