#include "WProgram.h"
#include <Bounce.h>

const int mode1Pin = 0;
const int mode3Pin = 1;

const int com1RotaryPinA = 23;
const int com1RotaryPinB = 22;
const int com2RotaryPinA = 8;
const int com2RotaryPinB = 9;
const int kneeboardRotaryPinA = 10;
const int kneeboardRotaryPinB = 11;

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

volatile boolean com1Fired;
volatile boolean com1Clockwise;
volatile boolean com2Fired;
volatile boolean com2Clockwise;
volatile boolean kneeboardFired;
volatile boolean kneeboardClockwise;

void rotaryCom1Changed() {
  if (digitalRead(com1RotaryPinA)) {
    com1Clockwise = !digitalRead(com1RotaryPinB);
  } else {
    com1Clockwise = digitalRead(com1RotaryPinB);
  }

  com1Fired = true;
}

void rotaryCom2Changed() {
  if (digitalRead(com2RotaryPinA)) {
    com2Clockwise = !digitalRead(com2RotaryPinB);
  } else {
    com2Clockwise = digitalRead(com2RotaryPinB);
  }

  com2Fired = true;
}

void rotaryKneeboardChanged() {
  if (digitalRead(kneeboardRotaryPinA)) {
    kneeboardClockwise = !digitalRead(kneeboardRotaryPinB);
  } else {
    kneeboardClockwise = digitalRead(kneeboardRotaryPinB);
  }

  kneeboardFired = true;
}

void setup() {
  // Serial.begin(9600);

  PanelMode1.useManualSend(true);
  PanelMode2.useManualSend(true);
  PanelMode3.useManualSend(true);

  pinMode(mode1Pin, INPUT_PULLUP);
  pinMode(mode3Pin, INPUT_PULLUP);

  pinMode(com1RotaryPinA, INPUT_PULLUP);
  pinMode(com1RotaryPinB, INPUT_PULLUP);
  pinMode(com2RotaryPinA, INPUT_PULLUP);
  pinMode(com2RotaryPinB, INPUT_PULLUP);
  pinMode(kneeboardRotaryPinA, INPUT_PULLUP);
  pinMode(kneeboardRotaryPinB, INPUT_PULLUP);

  attachInterrupt(com1RotaryPinA, rotaryCom1Changed, CHANGE);
  attachInterrupt(com2RotaryPinA, rotaryCom2Changed, CHANGE);
  attachInterrupt(kneeboardRotaryPinA, rotaryKneeboardChanged, CHANGE);

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
  // char buf[50];
  // sprintf(buf, "Button %d pressed", button);
  // Serial.println(buf);

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
  }

  zeroed = false;
  changed = true;
}

// the loop() methor runs over and over again,
// as long as the board has power
void loop() {
  Serial.println("loop");

  changed = false;

  if (!zeroed) {
    PanelMode1.reset();
    PanelMode2.reset();
    PanelMode3.reset();
    zeroed = true;
    changed = true;
  }

  unsigned int mode = 2;
  if (digitalRead(mode1Pin) == LOW) {
    mode = 1;
    // Serial.println("Mode 1");
  }
  if (digitalRead(mode3Pin) == LOW) {
    mode = 3;
    // Serial.println("Mode 3");
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
        case 3:
          key = action->key.primary;
          break;
        case 2:
          key = action->key.secondary;
          break;
        }

        if (state == HIGH) { // PRESSED
          // debounce
          if (millis() > (action->key.last_pressed+10) && (action->key.pressed == 0 || action->key.pressed != key)) {
            if (action->key.pressed != 0) {
              Keyboard.release(action->key.pressed);
            }
            Keyboard.press(key);
            Keyboard.release(key);
            action->key.pressed = key;
            action->key.last_pressed = millis();
          }
        } else {
          if (action->key.pressed > 0) {
            Keyboard.release(action->key.pressed);
            action->key.pressed = 0;
          }
        }

        break;
      }
    }

    digitalWrite(columnPins[x], LOW);
  }

  if (com1Fired) {
    if (com1Clockwise) {
      pressButton(com1RightRotateButton, mode);
      // Serial.println("COM1 right");
    } else {
      pressButton(com1LeftRotateButton, mode);
      // Serial.println("COM1 left");
    }
    com1Fired = false;
  }

  if (com2Fired) {
    if (com2Clockwise) {
      pressButton(com2RightRotateButton, mode);
      // Serial.println("COM2 right");
    } else {
      pressButton(com2LeftRotateButton, mode);
      // Serial.println("COM2 left");
    }
    com2Fired = false;
  }

  if (kneeboardFired) {
    if (kneeboardClockwise) {
      pressButton(kneeboardRightRotateButton, mode);
      // Serial.println("Kneeboard right");
    } else {
      pressButton(kneeboardLeftRotateButton, mode);
      // Serial.println("Kneeboard left");
    }
    kneeboardFired = false;
  }

  // send current button states
  if (changed) {
    PanelMode1.send_now();
    PanelMode2.send_now();
    PanelMode3.send_now();
  }

  // delay(1);
}


int main(void) {
	setup();
	while (1) {
		loop();
		yield();
	}
}