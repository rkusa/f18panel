#include "WProgram.h"
#include <Bounce.h>

const int mode1Pin = 23;
const int mode3Pin = 22;

Bounce mode1Button = Bounce(mode1Pin, 10);
Bounce mode3Button = Bounce(mode3Pin, 10);

const byte COLS = 7;
const byte ROWS = 6;

const int columnPins[COLS] = {
  11,
  10,
  9,
  8,
  7,
  6,
  5,
};

const int rowPins[ROWS] = {
  14,
  15,
  16,
  17,
  18,
  19,
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

struct KeyAction {
  int primary;
  int secondary;
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
    {Action::None, 0},
    {Action::Button, 1},
    {Action::Button, 2},
    {Action::Button, 3},
    {Action::Button, 4},
    {Action::Button, 5},
  },
  // Column 2
  {
    {Action::Button, 20},
    {Action::Key, {.key = KeyAction{KEY_DELETE, KEY_DELETE}}},
    {Action::Key, {.key = KeyAction{KEY_F7, KEYPAD_7}}},
    {Action::Key, {.key = KeyAction{KEY_F4, KEYPAD_4}}},
    {Action::Key, {.key = KeyAction{KEY_F1, KEYPAD_1}}},
    {Action::Button, 6},
  },
  // Column 3
  {
    {Action::Button, 19},
    {Action::Key, {.key = KeyAction{KEY_F10, KEYPAD_0}}},
    {Action::Key, {.key = KeyAction{KEY_F8, KEYPAD_8}}},
    {Action::Key, {.key = KeyAction{KEY_F5, KEYPAD_5}}},
    {Action::Key, {.key = KeyAction{KEY_F2, KEYPAD_2}}},
    {Action::Button, 7},
  },
  // Column 4
  {
    {Action::Button, 18},
    {Action::Key, {.key = KeyAction{KEYPAD_ENTER, KEYPAD_ENTER}}},
    {Action::Key, {.key = KeyAction{KEY_F9, KEYPAD_9}}},
    {Action::Key, {.key = KeyAction{KEY_F6, KEYPAD_6}}},
    {Action::Key, {.key = KeyAction{KEY_F3, KEYPAD_3}}},
    {Action::Button, 8},
  },
  // Column 4
  {
    {Action::Button, 17},
    {Action::Button, kneeboardPressButton},
    {Action::Button, com1RightRotateButton},
    {Action::Button, com1LeftRotateButton},
    {Action::Button, com1PressButton},
    {Action::Button, 9},
  },
  // Column 5
  {
    {Action::Button, 16},
    {Action::Button, kneeboardLeftRotateButton},
    {Action::Button, com2RightRotateButton},
    {Action::Button, com2LeftRotateButton},
    {Action::Button, com2PressButton},
    {Action::Button, 10},
  },
  // Column 6
  {
    {Action::Button, kneeboardRightRotateButton},
    {Action::Button, 15},
    {Action::Button, 14},
    {Action::Button, 13},
    {Action::Button, 12},
    {Action::Button, 11},
  },
};

void setup() {
  Serial.begin(9600);

  PanelMode1.useManualSend(true);
  PanelMode2.useManualSend(true);
  PanelMode3.useManualSend(true);

  // set pin mode for all rows and columns
  for (int i = 0; i < COLS; ++i) {
    pinMode(columnPins[i], OUTPUT);
  }
  for (int i = 0; i < ROWS; ++i) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }

  // // onboard LED
  // pinMode(13, OUTPUT);
}

// the loop() methor runs over and over again,
// as long as the board has power
void loop() {
  PanelMode1.reset();
  PanelMode2.reset();
  PanelMode3.reset();

  mode1Button.update();
  unsigned int mode = 2;
  if (mode1Button.fallingEdge()) {
    mode = 1;
  }
  if (mode3Button.fallingEdge()) {
    mode = 3;
  }

  for (auto x = 0; x < COLS; ++x) {
    digitalWrite(columnPins[x], HIGH);

    for (auto y = 0; y < ROWS; ++y) {
      if (digitalRead(rowPins[y]) == LOW) { // PRESSED
        auto action = actions[x][y];
        switch (action.kind) {
        case Action::None:
          break;
        case Action::Button:
          char buf[50];
          sprintf(buf, "Button %d pressed", action.button);
          Serial.println(buf);

          switch (mode) {
          case 1:
            PanelMode1.button(action.button, HIGH);
            break;
          case 2:
            PanelMode2.button(action.button, HIGH);
            break;
          case 3:
            PanelMode3.button(action.button, HIGH);
            break;
          }

          break;
        case Action::Key:

          switch (mode) {
          case 1:
          case 3:
            Keyboard.press(action.key.primary);
            Keyboard.release(action.key.primary);
            break;
          case 2:
            Keyboard.press(action.key.secondary);
            Keyboard.release(action.key.secondary);
            break;
          }

          break;
        }
      }
    }

    digitalWrite(columnPins[x], LOW);
  }

  // send current button states
  PanelMode1.send_now();
  PanelMode2.send_now();
  PanelMode3.send_now();

  // Note: there should not be any long delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  delay(100);
}


int main(void) {
	setup();
	while (1) {
		loop();
		yield();
	}
}