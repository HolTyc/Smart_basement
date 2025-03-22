#include <SPI.h>
#include "DHT.h"
#include <Wire.h>
#include "Adafruit_BMP085.h"
#include "MQ7.h"

#define pinDHT 5
#define typDHT22 DHT22

DHT myTemp(pinDHT, typDHT22);

Adafruit_BMP085 bmp180;

#define A_PIN 0
#define VOLTAGE 5

MQ7 mq7(A_PIN, VOLTAGE);
int korekce = 32;

const int A1_PIN = 1;
const int safePin = 6;
const int safeLed = 2;

bool flag = 0;
byte data = 0;
 
const int led = 8;
const int casovaniSPI = 150;
bool setupSPI = true;

byte tep = 0;
byte vlh = 0;
byte PPM = 0;
void setup() {
  pinMode(safePin, INPUT);
  pinMode(safeLed, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(MISO, OUTPUT);
  myTemp.begin();
  bmp180.begin();
  mq7.calibrate(); // Trva nejakou dobu
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  sei();
}

ISR (SPI_STC_vect) {
  data = SPDR;
  flag=1;
}

void loop() {
  if (!digitalRead(safePin)) {
	if (setupSPI==false) {
		//pinMode(MISO, OUTPUT);
		digitalWrite(safeLed,LOW);
		setupSPI=true;
	}
	
	tep = myTemp.readTemperature();
	vlh = myTemp.readHumidity();
	float value_MQ4 = analogRead(A1_PIN);	
	float uprava1 = value_MQ4 * 5 / 1023;
	float uprava2 = (5-uprava1)*1000/uprava1;
	PPM = pow(uprava2/945,-2.95)*1000;

	if (data) {
		for (int i = 0; i < 4; i++) {
			digitalWrite(led, HIGH);
			delay(500);
			digitalWrite(led, LOW);
			delay(500);
		}
		delay(2000);
	} else {
		digitalWrite(led, LOW);
	}
	if (flag) { 
		SPDR = vlh;
		delay(50);

		SPDR = tep;
		delay(casovaniSPI);

		SPDR = vlh;
		delay(casovaniSPI-50);
			
		SPDR = (bmp180.readPressure() + korekce*100)/1000.00;
		delay(casovaniSPI);

		SPDR = mq7.readPpm();
		delay(casovaniSPI-50);

		SPDR = PPM;
		delay(casovaniSPI);
		flag = 0;
	}
  } else {
	digitalWrite(safeLed, HIGH);
	//pinMode(MISO, INPUT);
	setupSPI=false;
	delay(1000);
  }

}
