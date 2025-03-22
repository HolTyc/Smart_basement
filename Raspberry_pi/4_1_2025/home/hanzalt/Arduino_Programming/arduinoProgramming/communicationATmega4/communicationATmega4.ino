#include <SPI.h>

const int safePin = 6;
const int safePinCom = 2;

bool flag = 0;
byte data = 0;
 
const int led = 5;
const int casovaniSPI = 150;

void setup() {
  pinMode(safePinCom, INPUT);
  pinMode(led, OUTPUT);
  pinMode(safePin, INPUT);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  sei();
}

ISR (SPI_STC_vect) {
  data = SPDR;
  flag=1;
}

void loop() {
  if (!digitalRead(safePin) and !digitalRead(safePinCom)) {
	digitalWrite(led,LOW);
  	if (data) {
		digitalWrite(led, HIGH);
	} else {
		digitalWrite(led, LOW);
	}
	if (flag) { 
		flag = 0;
	}
	//delay(1000);
  } else {
	digitalWrite(led, HIGH);
	delay(1000);
  }
}
