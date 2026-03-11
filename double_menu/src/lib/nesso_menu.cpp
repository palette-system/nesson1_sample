
#include "nesso_menu.h"


NessoMenu::NessoMenu() {
    this->_menu_canvas = new M5Canvas(&M5.Lcd);

    this->_menu_canvas->createSprite(240, 270);
    this->_menu_canvas->fillSprite(TFT_BLACK);
    this->_menu_canvas->setBaseColor(TFT_BLACK);
    this->_menu_canvas->setFont(&fonts::lgfxJapanGothic_24);
    this->_menu_canvas->setTextColor(TFT_YELLOW);
    this->_menu_canvas->setTextSize(1);

    this->btn_top.initButton(
        this->_menu_canvas,
        120, 68, // 表示位置 X, Y
        200, 100, // ボタンサイズ Width, Height
        TFT_YELLOW, // ふち色
        TFT_BLACK, // ボタン背景色
        TFT_YELLOW, // 文字色
        "Btn_top", // 文字
        1, 1 // 文字の表示倍率 width, height
    );
    this->btn_top.drawButton();
    this->btn_bttom.initButton(
        this->_menu_canvas,
        120, 203, // 表示位置 X, Y
        200, 100, // ボタンサイズ Width, Height
        TFT_YELLOW, // ふち色
        TFT_BLACK, // ボタン背景色
        TFT_YELLOW, // 文字色
        "Btn_bottom", // 文字
        1, 1 // 文字の表示倍率 width, height
    );
    this->btn_bttom.drawButton();
}

void NessoMenu::set_menu(char *menu_list_prm[], int menu_length_prm) {
    this->_menu_list = menu_list_prm;
    this->_menu_length = menu_length_prm;
    this->menu_select = 0;
    this->_slide_index = 0;
    this->_touch_end = 1;
    this->clicked = -1;
}



void NessoMenu::menu_loop() {
    int set_index = 0;
    int check_index = 0;
    this->clicked = -1;
    M5.update();
    auto t = M5.Touch.getDetail();

    if (this->_prev_y != t.y) {
        if (t.state == 9 || t.state == 13) {
            // スライド中
            this->_slide_index += this->_prev_y - t.y;
            if (this->_slide_index < 0) {
                this->_slide_index += 135;
                set_index = (this->menu_select - 1 < 0)? this->_menu_length - 1: this->menu_select - 1;
                this->btn_top.setLabelText(this->_menu_list[set_index]);
                this->btn_top.drawButton();
                this->btn_bttom.setLabelText(this->_menu_list[this->menu_select]);
                this->btn_bttom.drawButton();
                this->_slide_direction = 0;
            }
            if (this->_slide_index >= 135) {
                this->_slide_index -= 135;
                set_index = (this->menu_select + 1 > this->_menu_length - 1)? 0: this->menu_select + 1;
                this->btn_top.setLabelText(this->_menu_list[this->menu_select]);
                this->btn_top.drawButton();
                this->btn_bttom.setLabelText(this->_menu_list[set_index]);
                this->btn_bttom.drawButton();
                this->_slide_direction = 1;
            }
            this->_menu_canvas->pushSprite(&M5.Lcd, 0, 0 - this->_slide_index);
        }
        this->_prev_y = t.y;
    }

    if (this->_touch_end == 1) {
        check_index = (this->_slide_direction == 1)? this->_slide_pixel: 135 - this->_slide_pixel;
        if (this->_slide_index < check_index && this->_slide_index > 0) {
            this->_slide_index -= 20;
            if (this->_slide_index < 0) {
                this->_slide_index = 0;
                if (this->_slide_direction == 0) {
                    this->menu_select = (this->menu_select - 1 < 0)? this->_menu_length - 1: this->menu_select - 1;
                }
                set_index = (this->menu_select + 1 > this->_menu_length - 1)? 0: this->menu_select + 1;
                this->btn_top.setLabelText(this->_menu_list[this->menu_select]);
                this->btn_top.drawButton();
                this->btn_bttom.setLabelText(this->_menu_list[set_index]);
                this->btn_bttom.drawButton();
            }
            this->_menu_canvas->pushSprite(&M5.Lcd, 0, 0 - this->_slide_index);
        } else if (this->_slide_index > 0) {
            this->_slide_index += 20;
            if (this->_slide_index >= 135) {
                this->_slide_index = 0;
                if (this->_slide_direction == 1) {
                    this->menu_select = (this->menu_select + 1 > this->_menu_length - 1)? 0: this->menu_select + 1;
                }
                set_index = (this->menu_select + 1 > this->_menu_length - 1)? 0: this->menu_select + 1;
                this->btn_top.setLabelText(this->_menu_list[this->menu_select]);
                this->btn_top.drawButton();
                this->btn_bttom.setLabelText(this->_menu_list[set_index]);
                this->btn_bttom.drawButton();
            }
            this->_menu_canvas->pushSprite(&M5.Lcd, 0, 0 - this->_slide_index);
        }
        if (this->_slide_index == 0) {
            this->btn_top.setLabelText(this->_menu_list[this->menu_select]);
            this->btn_top.drawButton();
            this->_menu_canvas->pushSprite(&M5.Lcd, 0, 0);
            this->_touch_end = 0;
            this->_slide_direction = 1;
        }
    }

    if (t.state != this->_prev_touch_state) {
        if (t.state == 2 || t.state == 10 || t.state == 13) { // touch_end
            this->_touch_end = 1;
        }
        this->_prev_touch_state = t.state;
    }

    if (t.wasClicked()) {
        this->clicked = this->menu_select;
    }
}
