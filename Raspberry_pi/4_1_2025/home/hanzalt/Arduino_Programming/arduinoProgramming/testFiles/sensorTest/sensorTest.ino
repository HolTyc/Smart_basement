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

const int safePin = 6;
const int A1_PIN = 1;

//bool flag = 0;
//byte data = 0;
 
const int led = 8;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(safePin, INPUT);
//  pinMode(MISO, OUTPUT);
//  SPCR = (1<<SPE) | (1<<SPIE);
//  sei();
  myTemp.begin();
  bmp180.begin();
  Serial.println("Calibrating MQ7");
  mq7.calibrate();
  Serial.println("Calibration done!");
}

void loop() {
  if (!digitalRead(safePin)){
	float tep = myTemp.readTemperature();
	float vlh = myTemp.readHumidity();
	float value_MQ4 = analogRead(A1_PIN);	
	float uprava1 = value_MQ4 * 5 / 1023;
	float uprava2 = (5-uprava1)*1000/uprava1;
	float PPM = pow(uprava2/945,-2.95)*1000;

	Serial.print("Teplota DHT22: ");
	Serial.println(tep);
	Serial.print("Vlhkost DHT22: ");
	Serial.println(vlh);
	Serial.println("------------------------------");

	// výpis barometrického tlaku v hekto Pascalech
	Serial.print("Barometricky tlak BMP180: ");
	Serial.print((bmp180.readPressure() + korekce*100)/100.00);
	Serial.println(" hPa");
 	// výpis nadmořské výšky při započítání
	// běžné hodnoty tlaku 1013,25 hekto Pascalů
	Serial.print("Nadmorska vyska prepoctena BMP180: ");
	Serial.print(bmp180.readAltitude(bmp180.readPressure() + korekce*100));
	Serial.println(" metru ");
 	Serial.println("------------------------------");

	Serial.print("Hodnota CO MQ-7: ");
	Serial.print(mq7.readPpm());//prints the CO value
	Serial.println(" PPM");
	Serial.println("-----------------------------");

	Serial.print("Hodnota Methanu MQ-4: ");
	Serial.print(PPM);
	Serial.println(" PPM");
	Serial.println("-----------------------------");
	delay(2000);
  }  
}
