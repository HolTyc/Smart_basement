#include <Wire.h>
// Include the required Wire library for I2C<br>#include <Wire.h>
int LED = 8;
void setup() {
  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  pinMode (6, INPUT);
}
void loop() {
  if (digitalRead(6)==HIGH) {
	digitalWrite(LED,HIGH);
  } else {
	digitalWrite(LED,LOW);
  }
  delay(100);
}
