#include "WProgram.h"
#include <Bounce.h>

const int mode1Pin = 23;
const int mode3Pin = 22;

const int column1Pin = 11;
const int column2Pin = 10;
const int column3Pin = 9;
const int column4Pin = 8;
const int column5Pin = 7;
const int column6Pin = 6;
const int column7Pin = 5;

const int row1Pin = 14;
const int row2Pin = 15;
const int row3Pin = 16;
const int row4Pin = 17;
const int row5Pin = 18;
const int row6Pin = 19;

Bounce mode1Button = Bounce(mode1Pin, 10);
Bounce mode3Button = Bounce(mode3Pin, 10);

Bounce column1 = Bounce(column1Pin, 10);
Bounce column2 = Bounce(column2Pin, 10);
Bounce column3 = Bounce(column3Pin, 10);
Bounce column4 = Bounce(column4Pin, 10);
Bounce column5 = Bounce(column5Pin, 10);
Bounce column6 = Bounce(column6Pin, 10);
Bounce column7 = Bounce(column7Pin, 10);

Bounce row1 = Bounce(row1Pin, 10);
Bounce row2 = Bounce(row2Pin, 10);
Bounce row3 = Bounce(row3Pin, 10);
Bounce row4 = Bounce(row4Pin, 10);
Bounce row5 = Bounce(row5Pin, 10);
Bounce row6 = Bounce(row6Pin, 10);

#define PRESSED   1

const int com1PressButton = 21;
const int com1LeftRotateButton = 22;
const int com1RightRotateButton = 23;
const int com2PressButton = 24;
const int com2LeftRotateButton = 25;
const int com2RightRotateButton = 26;
const int kneeboardPressButton = 27;
const int kneeboardLeftRotateButton = 28;
const int kneeboardRightRotateButton = 29;

void setup() {
  Serial.begin(9600);

  PanelMode1.useManualSend(true);
  PanelMode2.useManualSend(true);
  PanelMode3.useManualSend(true);

  // set pin mode for all rows and columns
  pinMode(column1Pin, INPUT_PULLUP);
  pinMode(column2Pin, INPUT_PULLUP);
  pinMode(column3Pin, INPUT_PULLUP);
  pinMode(column4Pin, INPUT_PULLUP);
  pinMode(column5Pin, INPUT_PULLUP);
  pinMode(column6Pin, INPUT_PULLUP);
  pinMode(column7Pin, INPUT_PULLUP);
  pinMode(row1Pin, INPUT_PULLUP);
  pinMode(row2Pin, INPUT_PULLUP);
  pinMode(row3Pin, INPUT_PULLUP);
  pinMode(row4Pin, INPUT_PULLUP);
  pinMode(row5Pin, INPUT_PULLUP);
  pinMode(row6Pin, INPUT_PULLUP);

  // // onboard LED
  // pinMode(13, OUTPUT);
}

const void pressButton(const unsigned int num, const unsigned int mode) {
  switch (mode) {
  case 1:
    PanelMode1.button(num, PRESSED);
    break;
  case 2:
    PanelMode2.button(num, PRESSED);
    break;
  case 3:
    PanelMode3.button(num, PRESSED);
    break;
  }
}

// the loop() methor runs over and over again,
// as long as the board has power
void loop() {
  PanelMode1.reset();
  PanelMode2.reset();
  PanelMode3.reset();

  column1.update();
  column2.update();
  column3.update();
  column4.update();
  column5.update();
  column6.update();
  column7.update();
  row1.update();
  row2.update();
  row3.update();
  row4.update();
  row5.update();
  row6.update();

  // var panel = PanelMode1;
  unsigned int mode = 2;
  if (mode1Button.fallingEdge()) {
    mode = 1;
  }
  if (mode3Button.fallingEdge()) {
    mode = 3;
  }

  if (column1.fallingEdge()) {
    if (row1.fallingEdge()) {
      // unused
    }

    if (row2.fallingEdge()) {
      pressButton(1, mode);
    }

    if (row3.fallingEdge()) {
      pressButton(2, mode);
    }

    if (row4.fallingEdge()) {
      pressButton(3, mode);
    }

    if (row5.fallingEdge()) {
      pressButton(4, mode);
    }

    if (row6.fallingEdge()) {
      pressButton(5, mode);
    }
  }

  if (column2.fallingEdge()) {
    if (row1.fallingEdge()) {
      pressButton(20, mode);
    }

    if (row2.fallingEdge()) {
      Keyboard.press(KEY_DELETE);
      Keyboard.release(KEY_DELETE);
    }

    if (row3.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F7);
        Keyboard.release(KEY_F7);
        break;
      case 2:
        Keyboard.press(KEYPAD_7);
        Keyboard.release(KEYPAD_7);
        break;
      }
    }

    if (row4.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F4);
        Keyboard.release(KEY_F4);
        break;
      case 2:
        Keyboard.press(KEYPAD_4);
        Keyboard.release(KEYPAD_4);
        break;
      }
    }

    if (row5.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F1);
        Keyboard.release(KEY_F1);
        break;
      case 2:
        Keyboard.press(KEYPAD_1);
        Keyboard.release(KEYPAD_1);
        break;
      }
    }

    if (row6.fallingEdge()) {
      pressButton(6, mode);
    }
  }

  if (column3.fallingEdge()) {
    if (row1.fallingEdge()) {
      pressButton(19, mode);
    }

    if (row2.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F10);
        Keyboard.release(KEY_F10);
        break;
      case 2:
        Keyboard.press(KEYPAD_0);
        Keyboard.release(KEYPAD_0);
        break;
      }
    }

    if (row3.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F8);
        Keyboard.release(KEY_F8);
        break;
      case 2:
        Keyboard.press(KEYPAD_8);
        Keyboard.release(KEYPAD_8);
        break;
      }
    }

    if (row4.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F5);
        Keyboard.release(KEY_F5);
        break;
      case 2:
        Keyboard.press(KEYPAD_5);
        Keyboard.release(KEYPAD_5);
        break;
      }
    }

    if (row5.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F2);
        Keyboard.release(KEY_F2);
        break;
      case 2:
        Keyboard.press(KEYPAD_2);
        Keyboard.release(KEYPAD_2);
        break;
      }
    }

    if (row6.fallingEdge()) {
      pressButton(7, mode);
    }
  }

  if (column4.fallingEdge()) {
    if (row1.fallingEdge()) {
      pressButton(18, mode);
    }

    if (row2.fallingEdge()) {
      Keyboard.press(KEYPAD_ENTER);
      Keyboard.release(KEYPAD_ENTER);
    }

    if (row3.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F9);
        Keyboard.release(KEY_F9);
        break;
      case 2:
        Keyboard.press(KEYPAD_9);
        Keyboard.release(KEYPAD_9);
        break;
      }
    }

    if (row4.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F6);
        Keyboard.release(KEY_F6);
        break;
      case 2:
        Keyboard.press(KEYPAD_6);
        Keyboard.release(KEYPAD_6);
        break;
      }
    }

    if (row5.fallingEdge()) {
      switch (mode) {
      case 1:
      case 3:
        Keyboard.press(KEY_F3);
        Keyboard.release(KEY_F3);
        break;
      case 2:
        Keyboard.press(KEYPAD_3);
        Keyboard.release(KEYPAD_3);
        break;
      }
    }

    if (row6.fallingEdge()) {
      pressButton(8, mode);
    }
  }

  if (column5.fallingEdge()) {
    if (row1.fallingEdge()) {
      pressButton(17, mode);
    }

    if (row2.fallingEdge()) {
      pressButton(com1PressButton, mode);
    }

    if (row3.fallingEdge()) {
      pressButton(com1LeftRotateButton, mode);
    }

    if (row4.fallingEdge()) {
      pressButton(com1RightRotateButton, mode);
    }

    if (row5.fallingEdge()) {
      pressButton(kneeboardPressButton, mode);
    }

    if (row6.fallingEdge()) {
      pressButton(9, mode);
    }
  }

  if (column6.fallingEdge()) {
    if (row1.fallingEdge()) {
      pressButton(16, mode);
    }

    if (row2.fallingEdge()) {
      pressButton(com2PressButton, mode);
    }

    if (row3.fallingEdge()) {
      pressButton(com2LeftRotateButton, mode);
    }

    if (row4.fallingEdge()) {
      pressButton(com2RightRotateButton, mode);
    }

    if (row5.fallingEdge()) {
      pressButton(kneeboardLeftRotateButton, mode);
    }

    if (row6.fallingEdge()) {
      pressButton(10, mode);
    }
  }

  if (column7.fallingEdge()) {
    if (row1.fallingEdge()) {
      pressButton(kneeboardRightRotateButton, mode);
    }

    if (row2.fallingEdge()) {
      pressButton(15, mode);
    }

    if (row3.fallingEdge()) {
      pressButton(14, mode);
    }

    if (row4.fallingEdge()) {
      pressButton(13, mode);
    }

    if (row5.fallingEdge()) {
      pressButton(12, mode);
    }

    if (row6.fallingEdge()) {
      pressButton(11, mode);
    }
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