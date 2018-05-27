#include "WProgram.h"
// #include <Bounce.h>

void setup() {
  Serial.begin(9600);

  // PanelMode1.useManualSend(false);
  // PanelMode2.useManualSend(false);
  // PanelMode3.useManualSend(false);

  pinMode(2, INPUT_PULLUP);

  // onboard LED
  pinMode(13, OUTPUT);
}

// the loop() methor runs over and over again,
// as long as the board has power
void loop() {
  // digitalWrite(13, HIGH);   // set the LED on
  // PanelMode1.button(1, 1);
  // PanelMode2.button(2, 1);
  // PanelMode3.button(3, 1);
  // delay(1000); // wait for a second

  // digitalWrite(13, LOW);    // set the LED off
  // PanelMode1.button(1, 0);
  // PanelMode2.button(2, 0);
  // PanelMode3.button(3, 0);
  // delay(1000);

  // PanelMode1.button(1, 1);
  // delay(1000);
  // PanelMode1.button(1, 0);
  // PanelMode2.button(2, 1);
  // delay(1000);
  // PanelMode2.button(2, 0);
  // PanelMode3.button(3, 1);
  // delay(1000);
  // PanelMode3.button(3, 0);

  if (digitalRead(2) == LOW) {
      //The button is pressed!
    digitalWrite(13, HIGH);
   } else {
    digitalWrite(13, LOW);
   }
  delay(100);

  // update all the buttons
  // Note: there should not be any long delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.

}

int main(void) {
	setup();
	while (1) {
		loop();
		yield();
	}
}