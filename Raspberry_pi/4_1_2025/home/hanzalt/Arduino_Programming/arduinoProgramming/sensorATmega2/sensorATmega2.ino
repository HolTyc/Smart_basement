#include <SPI.h>
#include "DHT.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP3XX.h"
#include "SparkFun_SCD4x_Arduino_Library.h"
#include "MQ7.h"
#include "Servo.h"

//Defining temperature and humidity sensor
#define pinDHT 5
#define typDHT22 DHT22
DHT myTemp(pinDHT, typDHT22);

//Defining CO2 sensor 
SCD4x SCD40;

//Definig CO sensor
#define A_PIN 0
#define VOLTAGE 5
MQ7 mq7(A_PIN, VOLTAGE);

Adafruit_BMP3XX bmp;
#define SEALEVELPRESSURE_HPA (1013.25);

//Defining CH4 sensor
#define A1_PIN 1

//Defining safe led and communication led
#define safePin 6
#define safeLed 2
#define led 8
#define servo 9

//Variables for SPI communication
bool flag = 0;
byte data = 0;
const int casovaniSPI = 150;
bool setupSPI = true;

//Variables with sensor data to be send
byte tep = 0;
byte vlh = 0;
byte PPM = 0;
int CO2 = 0;

//Servo myservo;

void setup() {
  //myservo.attach(servo);
  pinMode(safePin, INPUT);
  pinMode(safeLed, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(MISO, OUTPUT);
  myTemp.begin();
  if (!bmp.begin_I2C()) {
	digitalWrite(safeLed, HIGH);
	digitalWrite(led, HIGH);
	while(1);
  }
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  if (SCD40.begin(false, true) == false) {
	digitalWrite(safeLed, HIGH);
    	digitalWrite(led, HIGH);
	delay(500);
	digitalWrite(safeLed, LOW);
    	digitalWrite(led, LOW);
	delay(500);
	digitalWrite(safeLed, HIGH);
    	digitalWrite(led, HIGH);
	delay(500);
	digitalWrite(safeLed, LOW);
    	digitalWrite(led, LOW);
	delay(150);
    	while (1);
  }
  SCD40.startLowPowerPeriodicMeasurement();
  mq7.calibrate(); // Takes some time
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
		pinMode(MISO, OUTPUT);
		digitalWrite(safeLed,LOW);
		setupSPI=true;
	}
	
	tep = myTemp.readTemperature();
	vlh = myTemp.readHumidity();
	float value_MQ4 = analogRead(A1_PIN);	
	float uprava1 = value_MQ4 * 5 / 1023;
	float uprava2 = (5-uprava1)*1000/uprava1;
	PPM = pow(uprava2/945,-2.95)*1000;
	if (!bmp.performReading()) {
		digitalWrite(safeLed, HIGH);
		return;
	}
	if (SCD40.readMeasurement()) {	
		CO2 = SCD40.getCO2() / 10.0;
	}
	//CO2 = 0;

	if (data) {
		digitalWrite(led, HIGH);
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
			
		SPDR = (bmp.pressure / 1000.0);
		delay(casovaniSPI);

		SPDR = mq7.readPpm();
		delay(casovaniSPI);
		
		SPDR = PPM;
		delay(casovaniSPI);

		SPDR = CO2;
		delay(casovaniSPI);
		flag = 0;
	}
  } else {
	digitalWrite(safeLed, HIGH);
	pinMode(MISO, INPUT);
	setupSPI=false;
	delay(1000);
  }

}
