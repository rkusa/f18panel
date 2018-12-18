#include "WProgram.h"
#include <Bounce.h>
#include <Encoder.h>

const int mode1Pin = 0;
const int mode3Pin = 1;
const int mode4Pin = 21;

const byte COLS = 6;
const byte ROWS = 6;

const int columnPins[COLS] = {
  19,
  18,
  17,
  16,
  15,
  14,
};

const int rowPins[ROWS] = {
  7,
  6,
  5,
  4,
  3,
  2,
};

const int com1PressButton = 21;
const int com1LeftRotateButton = 22;
const int com1RightRotateButton = 23;
const int com2PressButton = 24;
const int com2LeftRotateButton = 25;
const int com2RightRotateButton = 26;
const int kneeboardPressButton = 27;
const int kneeboardLeftRotateButton = 28;
const int kneeboardRightRotateButton = 29;

Encoder com1Encoder(23, 22);
Encoder com2Encoder(8, 9);
Encoder kneeboardEncoder(10, 11);
long oldCom1Position = 0;
long oldCom2Position = 0;
long oldKneeboardPosition = 0;

struct KeyAction {
  int primary;
  int secondary;
  int pressed;
  int last_pressed;
};

struct Action
{
  enum { None, Button, Key } kind;
  union {
    int unsigned button;
    KeyAction key;
  };
};

Action actions[COLS][ROWS] = {
  // Column 1
  {
    {Action::Button, 20},
    {Action::Button, 1},
    {Action::Button, 2},
    {Action::Button, 3},
    {Action::Button, 4},
    {Action::Button, 5},
  },
  // Column 2
  {
    {Action::Button, 19},
    {Action::Key, {.key = KeyAction{KEY_F12, KEY_DELETE, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F7, KEY_7, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F4, KEY_4, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F1, KEY_1, 0, 0}}},
    {Action::Button, 6},
  },
  // Column 3
  {
    {Action::Button, 18},
    {Action::Key, {.key = KeyAction{KEY_F10, KEY_0, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F8, KEY_8, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F5, KEY_5, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F2, KEY_2, 0, 0}}},
    {Action::Button, 7},
  },
  // Column 4
  {
    {Action::Button, 17},
    {Action::Key, {.key = KeyAction{KEYPAD_ENTER, KEYPAD_ENTER, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F9, KEY_9, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F6, KEY_6, 0, 0}}},
    {Action::Key, {.key = KeyAction{KEY_F3, KEY_3, 0, 0}}},
    {Action::Button, 8},
  },
  // Column 4
  {
    {Action::Button, 16},
    {Action::Button, kneeboardPressButton},
    {Action::None, 0},
    {Action::Button, com2PressButton},
    {Action::Button, com1PressButton},
    {Action::Button, 9},
  },
  // Column 6
  {
    {Action::Button, 15},
    {Action::Button, 14},
    {Action::Button, 13},
    {Action::Button, 12},
    {Action::Button, 11},
    {Action::Button, 10},
  },
};

void setup() {
  #if defined(USB_SERIAL)
  Serial.begin(9600);
  #endif

  #if defined(USB_PANEL)
  PanelMode1.useManualSend(true);
  PanelMode2.useManualSend(true);
  PanelMode3.useManualSend(true);
  PanelMode4.useManualSend(true);
  #endif

  pinMode(mode1Pin, INPUT_PULLUP);
  pinMode(mode3Pin, INPUT_PULLUP);
  pinMode(mode4Pin, INPUT_PULLUP);

  // set pin mode for all rows and columns
  for (int i = 0; i < COLS; ++i) {
    pinMode(columnPins[i], OUTPUT);
  }
  for (int i = 0; i < ROWS; ++i) {
    pinMode(rowPins[i], INPUT_PULLDOWN);
  }
}

auto changed = false;
auto zeroed = true;

void pressButton(const int unsigned button, const unsigned int mode) {
  #if defined(USB_SERIAL)
  char buf[50];
  sprintf(buf, "Button %d pressed (mode %d)", button, mode);
  Serial.println(buf);
  #endif

  #if defined(USB_PANEL)
  switch (mode) {
  case 1:
    PanelMode1.button(button, HIGH);
    break;
  case 2:
    PanelMode2.button(button, HIGH);
    break;
  case 3:
    PanelMode3.button(button, HIGH);
    break;
  case 4:
    PanelMode4.button(button, HIGH);
    break;
  }
  #endif

  zeroed = false;
  changed = true;
}

// the loop() methor runs over and over again,
// as long as the board has power
void loop() {
  changed = false;

  if (!zeroed) {
    #if defined(USB_PANEL)
    PanelMode1.reset();
    PanelMode2.reset();
    PanelMode3.reset();
    PanelMode4.reset();
    #endif
    zeroed = true;
    changed = true;
  }

  unsigned int mode = 4;
  if (digitalRead(mode4Pin) == HIGH) {
    mode = 2;
    if (digitalRead(mode1Pin) == LOW) {
      mode = 1;
    }
    if (digitalRead(mode3Pin) == LOW) {
      mode = 3;
    }
  }

  for (auto x = 0; x < COLS; ++x) {
    digitalWrite(columnPins[x], HIGH);

    for (auto y = 0; y < ROWS; ++y) {
      auto action = &actions[x][y];
      auto state = digitalRead(rowPins[y]);

      switch (action->kind) {
      case Action::None:
        break;
      case Action::Button:
        if (state == HIGH) { // PRESSED
          pressButton(action->button, mode);
        }

        break;
      case Action::Key:
        int key;
        switch (mode) {
        case 1:
        case 2:
        case 3:
          key = action->key.primary;
          break;
        case 4:
          key = action->key.secondary;
          break;
        }

        #if defined(USB_PANEL)
        if (state == HIGH) { // PRESSED
          // debounce
          if (millis() > (action->key.last_pressed+10) && (action->key.pressed == 0 || action->key.pressed != key)) {
            if (action->key.pressed != 0) {
              Keyboard.release(action->key.pressed);
            }
            Keyboard.press(key);
            action->key.pressed = key;
            action->key.last_pressed = millis();
          }
        } else {
          if (action->key.pressed > 0) {
            Keyboard.release(action->key.pressed);
            action->key.pressed = 0;
          }
        }
        #endif

        break;
      }
    }

    digitalWrite(columnPins[x], LOW);
  }

  long newPosition = com2Encoder.read();
  long diff = oldCom2Position - newPosition;
  if (diff >= 4) {
    pressButton(com2RightRotateButton, mode);
    oldCom2Position = newPosition;
  } else if (diff <= -4) {
    pressButton(com2LeftRotateButton, mode);
    oldCom2Position = newPosition;
  }

  newPosition = com1Encoder.read();
  diff = oldCom1Position - newPosition;
  if (diff >= 4) {
    pressButton(com1RightRotateButton, mode);
    oldCom1Position = newPosition;
  } else if (diff <= -4) {
    pressButton(com1LeftRotateButton, mode);
    oldCom1Position = newPosition;
  }

  newPosition = kneeboardEncoder.read();
  diff = oldKneeboardPosition - newPosition;
  if (diff >= 4) {
    pressButton(kneeboardRightRotateButton, mode);
    oldKneeboardPosition = newPosition;
  } else if (diff <= -4) {
    pressButton(kneeboardLeftRotateButton, mode);
    oldKneeboardPosition = newPosition;
  }

  // send current button states
  #if defined(USB_PANEL)
  if (changed) {
    PanelMode1.send_now();
    PanelMode2.send_now();
    PanelMode3.send_now();
    PanelMode4.send_now();
  }
  #endif

  delay(8);
}


int main(void) {
	setup();
	while (1) {
		loop();
		yield();
	}
}