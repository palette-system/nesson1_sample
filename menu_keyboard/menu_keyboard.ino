
#include <M5GFX.h>
#include <M5Unified.h>

// BLEキーボードはこちらのライブラリを使わせて頂きました。
// https://github.com/siroitori0413/ESP32-BLE-Keyboard
#include "src/lib/BleKeyboardC6.h"
BleKeyboard bleKeyboard;

static M5Canvas canvas(&M5.Lcd);
LGFX_Button b_left;
LGFX_Button b_right;

const char *menu_list[] = {
  "Ctrl + A",
  "Ctrl + C",
  "Ctrl + V",
  "Enter",
  "Shift",
  "Delete",
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
};

int menu_index = 0; // 今選択しているメニュー
int menu_len = 13; // menu_list の要素数

void pressKey() {
  if (menu_index == 0) {
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(0x61); // 
  } else if (menu_index == 1) {
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(0x63); // C
  } else if (menu_index == 2) {
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(0x76); // V
  } else if (menu_index == 3) {
    bleKeyboard.press(KEY_RETURN); // Enter
  } else if (menu_index == 4) {
    bleKeyboard.press(KEY_LEFT_SHIFT); // Shift
  } else if (menu_index == 5) {
    bleKeyboard.press(KEY_DELETE); // Delete
  } else if (menu_index == 6) {
    bleKeyboard.press(0x61); // A
  } else if (menu_index == 7) {
    bleKeyboard.press(0x62); // B
  } else if (menu_index == 8) {
    bleKeyboard.press(0x63); // C
  } else if (menu_index == 9) {
    bleKeyboard.press(0x64); // D
  } else if (menu_index == 10) {
    bleKeyboard.press(0x65); // E
  } else if (menu_index == 11) {
    bleKeyboard.press(0x66); // F
  } else if (menu_index == 12) {
    bleKeyboard.press(0x67); // G
  }

}

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);

  M5.Display.setRotation(1);
  canvas.createSprite(240, 270);
  canvas.fillSprite(TFT_BLACK);
  canvas.setBaseColor(TFT_BLACK);
  canvas.setFont(&fonts::lgfxJapanGothic_24);
  canvas.setTextColor(TFT_YELLOW);
  canvas.setTextSize(1);


  b_left.initButton(&canvas, 120, 68, 200, 100, TFT_YELLOW, TFT_BLACK, TFT_YELLOW, "Btn_top", 1, 1);
  b_left.drawButton();
  b_right.initButton(&canvas, 120, 203, 200, 100, TFT_YELLOW, TFT_BLACK, TFT_YELLOW, "Btn_downe", 1, 1);
  b_right.drawButton();
  

  auto& ioe0 = M5.getIOExpander(0);
  ioe0.digitalWrite(5, false); // lora OFF
  ioe0.digitalWrite(6, false); // lora OFF
  ioe0.digitalWrite(7, false); // lora OFF

  auto& ioe1 = M5.getIOExpander(1);
  ioe1.digitalWrite(5, false); // glove 5v 出力OFF
  ioe1.digitalWrite(6, true); // LCDバックライト
  ioe1.digitalWrite(7, false); // 


  M5.Power.setBatteryCharge(true);
  M5.Power.setChargeCurrent(100);
  M5.Power.setChargeVoltage(4200);
  M5.Power.setLed(50);

  pinMode(GPIO_NUM_3, INPUT_PULLUP);
  bleKeyboard.setName("NessoN1");
  bleKeyboard.begin();
}

int slide_index = 1;
int slide_direction = 0;
int slide_pixel = 30; // 50ピクセル以上スライドしたら隣のメニューに移る
int btn_stat = 0;
int touch_start_x = 0;
int prev_x = -1;
int touch_end = 1;
int prev_touch_state;
int send_state = 0;

void loop(void) {
  M5.update();
  auto t = M5.Touch.getDetail();
  int level = M5.Power.getBatteryLevel();
  int set_index = 0;
  int check_index = 0;
  

  if (prev_x != t.y) {
    if (t.state == 9 || t.state == 13) {
      // スライド中
      slide_index += prev_x - t.y;
      if (slide_index < 0) {
        slide_index += 135;
        set_index = (menu_index - 1 < 0)? menu_len - 1: menu_index - 1;
        b_left.setLabelText(menu_list[set_index]);
        b_left.drawButton();
        b_right.setLabelText(menu_list[menu_index]);
        b_right.drawButton();
        slide_direction = 0;
      }
      if (slide_index >= 135) {
        slide_index -= 135;
        set_index = (menu_index + 1 > menu_len - 1)? 0: menu_index + 1;
        b_left.setLabelText(menu_list[menu_index]);
        b_left.drawButton();
        b_right.setLabelText(menu_list[set_index]);
        b_right.drawButton();
        slide_direction = 1;
      }
      canvas.pushSprite(&M5.Lcd, 0, 0 - slide_index);
    }
    prev_x = t.y;
  }

  if (touch_end == 1) {
    check_index = (slide_direction == 1)? slide_pixel: 135 - slide_pixel;
    if (slide_index < check_index && slide_index > 0) {
      slide_index -= 20;
      if (slide_index < 0) {
        slide_index = 0;
        if (slide_direction == 0) {
          menu_index = (menu_index - 1 < 0)? menu_len - 1: menu_index - 1;
        }
        set_index = (menu_index + 1 > menu_len - 1)? 0: menu_index + 1;
        b_left.setLabelText(menu_list[menu_index]);
        b_left.drawButton();
        b_right.setLabelText(menu_list[set_index]);
        b_right.drawButton();
      }
      canvas.pushSprite(&M5.Lcd, 0, 0 - slide_index);
    } else if (slide_index > 0) {
      slide_index += 20;
      if (slide_index >= 135) {
        slide_index = 0;
        if (slide_direction == 1) {
          menu_index = (menu_index + 1 > menu_len - 1)? 0: menu_index + 1;
        }
        set_index = (menu_index + 1 > menu_len - 1)? 0: menu_index + 1;
        b_left.setLabelText(menu_list[menu_index]);
        b_left.drawButton();
        b_right.setLabelText(menu_list[set_index]);
        b_right.drawButton();
      }
      canvas.pushSprite(&M5.Lcd, 0, 0 - slide_index);
    }
    if (slide_index == 0) {
      b_left.setLabelText(menu_list[menu_index]);
      b_left.drawButton();
      canvas.pushSprite(&M5.Lcd, 0, 0);
      touch_end = 0;
      slide_direction = 1;
    }
  }

  if (t.state != prev_touch_state) {
    if (t.state == 2 || t.state == 10 || t.state == 13) { // touch_end
      touch_end = 1;
    }
  }
  if (t.wasClicked()) {
    pressKey();
    // delay(80);
    bleKeyboard.releaseAll();
  }
  prev_touch_state = t.state;

  if (M5.BtnA.isPressed()) {
    if (send_state == 0) {
      pressKey();
      send_state = 1;
    }

  } else {
    if (send_state == 1) {
      bleKeyboard.releaseAll();
      send_state = 0;
    }
  }
}