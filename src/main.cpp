#include "WProgram.h"
#include <Bounce.h>


const int rotary_button_pin = 14;
const int rotary_output_a = 15;
const int rotary_output_b = 16;

const int poti_analog_pin = 3;
const int hall_analog_pin = 4;

// create Bounce objects for each button, which deals with contact chatter or "bounce", and
// it makes detecting changes very simple. 10 = 10 ms debounce time
Bounce button12 = Bounce(12, 10);

Bounce button21 = Bounce(21, 10);
Bounce button22 = Bounce(22, 10);
Bounce button23 = Bounce(23, 10);

// Bounce button14 = Bounce(13, 10);
// Bounce button14 = Bounce(14, 10);

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 15;
int encoderPin2 = 16;
Bounce button_rotary = Bounce(rotary_button_pin, 10);

int rotary_position = -1;
int last_rotary_state;
int rotary_direction = 0;

void setup() {
  Serial.begin(9600);

  // onboard LED
  // pinMode(13, OUTPUT);

  // button2
  pinMode(12, INPUT_PULLUP);

  // 6 step rotary
  pinMode(21, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);

  // rotary
  pinMode(rotary_button_pin, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  last_rotary_state = digitalRead(rotary_output_a); // get initial state
}

// the loop() methor runs over and over again,
// as long as the board has power
void loop() {
  // digitalWrite(ledPin, HIGH);   // set the LED on
  // delay(100);                   // wait for a second
  // digitalWrite(ledPin, LOW);    // set the LED off
  // delay(100);

  // update all the buttons
  // Note: there should not be any long delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button12.update();

  button21.update();
  button22.update();
  button23.update();

  button_rotary.update();

  // Check each button for "falling" (= high = not pressed, cause pullup resistor) edge
  if (button12.fallingEdge()) {
    Serial.println("Button 12 pressed");
    // Keyboard.press(KEY_F2);
    // Keyboard.release(KEY_F2);
  }

  // 6 step rotary
  if (rotary_position != -1) {
    // Keyboard.release(rotary_position);
    rotary_position = -1;
  }

  if (button23.fallingEdge()) {
    Serial.println("[6 step rotary] 1 active");
    // Keyboard.press(rotary_position = KEY_F4);
  } else if (button22.fallingEdge()) {
    Serial.println("[6 step rotary] 2 active");
    // Keyboard.press(rotary_position = KEY_F5);
  } else if (button21.fallingEdge()) {
    Serial.println("[6 step rotary] 3 active");
    // Keyboard.press(rotary_position = KEY_F6);
  }

  // rotary encoder
  if (button_rotary.fallingEdge()) {
    Serial.println("[rotary encoder] pressed");
  }

  int rotary_state = digitalRead(rotary_output_a);
  if (rotary_state != last_rotary_state) {
    if (rotary_state != digitalRead(rotary_output_b)) {
      rotary_direction++;
      if (rotary_direction == 2) {
        Serial.println("[rotary encoder] clockwise");
        rotary_direction = 0;
      }
    } else {
      rotary_direction--;
      if (rotary_direction == -2) {
        Serial.println("[rotary encoder] counterclockwise");
        rotary_direction = 0;
      }
    }
    last_rotary_state = rotary_state;
  }


  // potentiometer
  // Serial.print("[potentiometer]: ");
  // Serial.println(analogRead(poti_analog_pin));
  // delay(250);


  // hall effect sensor
  // analogReadResolution(16);
  // int raw = analogRead(hall_analog_pin);
  // int mapped = map(
  //   raw, // the value to map
  //   50350, // fromLow: the lower bound of the value's current range
  //   65535, // fromHigh: the upper bound of the value's current range
  //   0, // toLow: the lower bound of the value's target range
  //   4096 // toHigh: the upper bound of the value's target range
  // );
  // Serial.print("[hall]: "); Serial.println(mapped);
  // delay(250);
}

int main(void) {
	setup();
	while (1) {
		loop();
		yield();
	}
}