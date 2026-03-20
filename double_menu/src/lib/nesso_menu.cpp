
#include "nesso_menu.h"


NessoMenu::NessoMenu() {
    this->_menu_canvas = new M5Canvas(&M5.Lcd);

    this->_menu_canvas->createSprite(240, 270);
    this->_menu_canvas->fillSprite(TFT_BLACK);
    this->_menu_canvas->setBaseColor(TFT_BLACK);
    this->_menu_canvas->setFont(&fonts::lgfxJapanGothic_24);
    this->_menu_canvas->setTextColor(TFT_YELLOW);
    this->_menu_canvas->setTextSize(1);
	
	M5.Display.setFont(&fonts::lgfxJapanGothic_24);
	M5.Display.setTextColor(TFT_YELLOW);

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
	this->_prev_battery_level = 0;
}

void NessoMenu::set_menu(char *menu_list_prm[], int menu_length_prm) {
    this->_menu_list = menu_list_prm;
    this->_menu_length = menu_length_prm;
    this->menu_select = 0;
    this->_slide_index = 0;
    this->_touch_end = 1;
    this->clicked = -1;
    this->_touch_start = millis();
}

int vx, vy, vt;

void NessoMenu::menu_loop() {
    int set_index = 0;
    int check_index = 0;
    unsigned long touch_time;
    this->clicked = -1;
    M5.update();
    auto t = M5.Touch.getDetail();
    if ((t.y < 0 || t.y > 135 || t.x < 0 || t.x > 240)
        && this->_battery_level == this->_prev_battery_level) {
        return;
    }
	if (this->_battery_level != this->_prev_battery_level) {
		this->_prev_battery_level = this->_battery_level;
		this->_touch_end = 1;
	}
    if (t.state == 3) { // タッチスタート
        this->_touch_start = millis();
    }
	if (t.state == 0) {
		if (this->_display_on > 200) {
			delay(10);
			auto& ioe1 = M5.getIOExpander(1);
			// ioe1.digitalWrite(1, true); // LCDリセット
			ioe1.digitalWrite(6, false); // LCDバックライト
			delay(10);
			this->_display_on = 0;
		} else if (this->_display_on > 0) {
			this->_display_on++;
		}
	} else {
		if (this->_display_on == 0) {
			delay(10);
			auto& ioe1 = M5.getIOExpander(1);
			// ioe1.digitalWrite(1, false); // LCDリセット
			ioe1.digitalWrite(6, true); // LCDバックライト
			delay(10);
		}
		this->_display_on = 1;
	}

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
    M5.Display.drawString(String(_battery_level) + "%", 190, 3);
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
    M5.Display.drawString(String(_battery_level) + "%", 190, 3);
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
    M5.Display.drawString(String(_battery_level) + "%", 190, 3);
        }
        if (this->_slide_index == 0) {
            this->btn_top.setLabelText(this->_menu_list[this->menu_select]);
            this->btn_top.drawButton();
            this->_menu_canvas->pushSprite(&M5.Lcd, 0, 0);
    M5.Display.drawString(String(_battery_level) + "%", 190, 3);
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
        touch_time = millis() - this->_touch_start;
        if (touch_time > 5 && touch_time < 200) {
        	vx = t.x;
        	vy = t.y;
        	vt = touch_time;
            this->clicked = this->menu_select;
        }
    }
}
