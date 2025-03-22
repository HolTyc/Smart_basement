#include <SPI.h>
#include <Wire.h>
#include "Keypad.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool flag = 0;
byte data = 0;
 
const int led = A0;
const int masterOrder = A1;
const int safePin = A2;
const int safeLed = A3;

char keys[4][4] = {
  {16, 1, 2, 3},
  {4, 5, 6, 7},
  {8, 9, 10, 11},
  {12, 13, 14, 15}
};

char keyCodes[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinyRadku[4] = {9,8,7,6};
byte pinySloupcu[4] = {5, 4, 3, 2};

Keypad klavesnice = Keypad(makeKeymap(keys),pinyRadku, pinySloupcu, 4, 4);

char klavesa = ' ';
char decodedKey = ' ';
byte matice = 0;
const int casovaniSPI = 150;
int sensorData = 0;
int scrollMenu = 1;
bool aktivaceSensor = 0;
bool setupSPI = true;

void nastaveniMatice() {
  int radek = klavesa/4;
  int sloupec = klavesa%4;
  decodedKey = keyCodes[radek][sloupec];
  radek++;
  sloupec++;
  radek=128/pow(2, radek-1);
  sloupec=8/pow(2, sloupec-1);
  matice = radek + sloupec;
  if (klavesa==16) {
	matice = 136;
	decodedKey = keyCodes[0][0];
  }
}

void napsaniDatSensoru() {
  String sensorZpravy[6] = {"Teplota: ", "Vlhkost: ", "Tlak: ", "c[CO]: ", "c[CH4]: "};
  String jednotky[6] = {"C", "%", "hPa", "PPM", "PPM"};
  if (decodedKey-'0'==3) {
	sensorData = sensorData * 10;
  }
  if (decodedKey - '0' >= 1 and decodedKey - '0' <= 5) {
	lcd.clear();
	lcd.print(sensorZpravy[decodedKey - '0' - 1]);
	lcd.print(sensorData);
	lcd.print(jednotky[decodedKey - '0' - 1]);	
  }
  aktivaceSensor = 0;
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(safeLed, OUTPUT);
  pinMode(masterOrder, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(safePin, INPUT);
  lcd.begin(16,2);
  lcd.backlight();
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  sei();
  lcd.clear();
  lcd.print("Zdravim");
  lcd.setCursor(0,1);
  lcd.print("Zmackni *");
}

ISR (SPI_STC_vect) {
  data = SPDR;
}

void loop() {
  klavesa = klavesnice.getKey();
  if (scrollMenu==1 and aktivaceSensor == 1) {
	lcd.clear();
  	lcd.print("Teplota  Vlhkost");
 	lcd.setCursor(0,1);
  	lcd.print("   1        2   "); 
	scrollMenu = -1;
  } else if (scrollMenu==2 and aktivaceSensor == 1) {
	lcd.clear();
  	lcd.print("Tlak   CO   CH4");
 	lcd.setCursor(0,1);
  	lcd.print(" 3     4     5 "); 
	scrollMenu = -2;
  }
  if (!digitalRead(safePin)) {
	if (setupSPI==false) {
		//pinMode(MISO, OUTPUT);
		digitalWrite(safeLed,LOW);
		setupSPI=true;
	}
  	if (klavesa) {
		digitalWrite(led,HIGH);
		lcd.clear();
  		lcd.print("Klavesa: ");
		nastaveniMatice();
  		lcd.print(decodedKey);
		digitalWrite(masterOrder, HIGH);
		SPDR = matice;
		delay(150);
		sensorData = data;
		delay(150);
		matice = 0;
		digitalWrite(masterOrder, LOW);
		digitalWrite(led, LOW);
		//delay(250);
		if (aktivaceSensor==1) {
			if (decodedKey=='#') {
				if (scrollMenu == -1) {
					scrollMenu = 2;
				} else if (scrollMenu == -2) {
					scrollMenu = 1;
				}
			} else {
				napsaniDatSensoru();
				delay(5000);
			}
			lcd.clear();
			lcd.setCursor(0,1);
			lcd.print("Zmackni *");
			scrollMenu = abs(scrollMenu);
		} else {
			lcd.setCursor(0,1);
			lcd.print("Zmackni *");
			if (decodedKey=='*') {
				aktivaceSensor=1;
			}
		}
  	} else {
		digitalWrite(led, LOW);
  	}
  } else {
	digitalWrite(safeLed, HIGH);
	//pinMode(MISO, INPUT);
	setupSPI=false;
	delay(1000);
  }
}

