
#include <M5GFX.h>
#include <M5Unified.h>

#include <BleKeyboardC6.h>
BleKeyboard bleKeyboard;

#include "src/lib/nesso_menu.h"

NessoMenu menu_obj;

const char *key_list[] = {
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
  "もどる"
};

const char *keyboard_list[] = {
  "Menu A",
  "Menu B",
  "Menu C"
};

int menu_type;

void pressKey(int menu_index) {
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
  menu_type = 0;
  menu_obj.set_menu((char **)&key_list, 14);

  auto& ioe0 = M5.getIOExpander(0);
  ioe0.digitalWrite(5, false); // lora OFF
  ioe0.digitalWrite(6, false); // lora OFF
  ioe0.digitalWrite(7, false); // lora OFF

  auto& ioe1 = M5.getIOExpander(1);
  ioe1.digitalWrite(5, false); // glove 5v 出力
  ioe1.digitalWrite(6, true); // LCDバックライト
  ioe1.digitalWrite(7, false); // LCDバックライト

  M5.Power.setBatteryCharge(true);
  M5.Power.setChargeCurrent(100);
  M5.Power.setChargeVoltage(4200);
  M5.Power.setLed(50);

  pinMode(GPIO_NUM_3, INPUT_PULLUP);
  bleKeyboard.setName("NessoN1");
  bleKeyboard.begin();
}


void loop(void) {
  int level = M5.Power.getBatteryLevel();
  menu_obj.menu_loop();
  if (menu_type == 0) {
    if (menu_obj.clicked >= 0) {
      if (menu_obj.clicked == 13) {
        menu_type = 1;
        menu_obj.set_menu((char **)&keyboard_list, 3);
      } else {
        pressKey(menu_obj.clicked);
        bleKeyboard.releaseAll();
      }
    }
  } else if (menu_type == 1) {
    if (menu_obj.clicked >= 0) {
      menu_type = 0;
      menu_obj.set_menu((char **)&key_list, 14);
    }
  }
  
}