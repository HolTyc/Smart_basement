#include <SPI.h>
#include "RF24.h"

RF24 radio(7, 8);

const int CE = 10;
const int CE3 = 3;
const int safePin = 6;
const int dataLed = 2;
const int safeLed = 4;
const int masterOrder = 5;
const int led = 8;
const int showLED1 = A5;
const int showLED2 = A4;
const int showLED3 = A3;
const int showLED4 = A2;
const int showLED5 = A1;
const int showLED6 = A0;

const int casovaniSPI = 150;

byte data = 0;

int tep = 0;
int vlh = 0;
int tlak = 0;
int uhlikPPM = 0;
int methanPPM = 0;
int CO2 = 0;

bool setupSPI = false;
unsigned long previousMillis = 0;
unsigned long halfHourWait = 0;

unsigned long halfHour = 600000;
unsigned long intervalSensor = 120000;//3600000;
const byte addresses[3][6] = {"00001", "00002"};
int counterPohyb = 1000;
bool pohyb = false;

char keys[4][4] = {
  {'A','3','2','1'},
  {'B','6','5','4'},
  {'C','9','8','7'},
  {'D','#','0','*'}
};
char klavesa = ' ';
int x = 0;

int log2(int a) {
  return log(a) / log(2);
}

void prevedeniDatNaKlavesu(int data) {
  int sloupec = 0;
  int radek = 0;
  if (data>=128) {
	radek = 0;
	sloupec = data-128;
	sloupec = log2(sloupec);
  } else if (data>=64) {
	radek = 1;
	sloupec = data-64;
	sloupec = log2(sloupec);
  } else if (data>=32) {
	radek = 2;
	sloupec = data-32;
	sloupec = log2(sloupec);
  } else if (data>=16) {
	radek = 3;
	sloupec = data-16;
	sloupec = log2(sloupec);
  }
  klavesa = keys[radek][sloupec];
}

void odeslaniDatSensoru() {
  int sensorDataRaw[7] = {tep, vlh, tlak/10, uhlikPPM, methanPPM, CO2/10};
  if ((klavesa - '0') >=1 and (klavesa - '0') <= 6) {
  	data = SPI.transfer(sensorDataRaw[(klavesa-'0')-1]);
  } else {
	data = SPI.transfer(0);
  }
}
void sensor() {
	//pinMode(7, OUTPUT);
	//radio.powerDown();
	//delay(500);
	digitalWrite(dataLed,HIGH);
	digitalWrite(CE,LOW);
	//SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
	SPI.transfer(1);
	delay(50);

	data = SPI.transfer(1);
	vlh = data;
	delay(casovaniSPI);	

	data = SPI.transfer(2);
	tep = data;
	delay(casovaniSPI);

	data = SPI.transfer(3);
	tlak = data * 10; // tlak se musi vynasobit deseti
	delay(casovaniSPI);

	data = SPI.transfer(4);
	uhlikPPM = data;
	delay(casovaniSPI);
	
	data = SPI.transfer(5);
	methanPPM = data;
	delay(casovaniSPI);

	data = SPI.transfer(0);
	CO2 = data * 10;
	delay(casovaniSPI);
	/*
	if ((vlh == 1 or tlak*10<=500) and x<=2) {
		Serial.print("pokus cislo: ");
		Serial.println(x);
		delay(5000);
		x++;
		sensor();
	} else {
		x = 0;
	}
	*/
	digitalWrite(CE,HIGH);
	digitalWrite(dataLed, LOW);

	//radio.powerUp();
/*
	Serial.println("Konec prenosu dat");

	Serial.print("Teplota DHT22: ");
	Serial.println(tep);
	Serial.print("Vlhkost DHT22: ");
	Serial.println(vlh);
	Serial.println("------------------------------");

	// výpis barometrického tlaku v hekto Pascalech
	Serial.print("Barometricky tlak BMP180: ");
	Serial.print(tlak);
	Serial.println(" hPa");
 	// výpis nadmořské výšky při započítání
	// běžné hodnoty tlaku 1013,25 hekto Pascal
 	Serial.println("------------------------------");

	Serial.print("Hodnota CO MQ-7: ");
	Serial.print(uhlikPPM);//prints the CO value
	Serial.println(" PPM");
	Serial.println("-----------------------------");

	Serial.print("Hodnota Methanu MQ-4: ");
	Serial.print(methanPPM);
	Serial.println(" PPM");
	Serial.println("-----------------------------");

	Serial.print("Hodnota CO2 SCD41: ");
	Serial.print(CO2);
	Serial.println(" PPM");
	Serial.println("-----------------------------");
*/
}
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  // Showing LEDs
  pinMode(showLED1, OUTPUT);
  pinMode(showLED2, OUTPUT);
  pinMode(showLED3, OUTPUT);
  pinMode(showLED4, OUTPUT);
  pinMode(showLED5, OUTPUT);
  pinMode(showLED6, OUTPUT);
  // Turning showing LEDs on
  digitalWrite(showLED1, LOW);
  digitalWrite(showLED2, LOW);
  digitalWrite(showLED3, LOW);
  digitalWrite(showLED4, LOW);
  digitalWrite(showLED5, LOW);
  digitalWrite(showLED6, LOW);

  pinMode(CE, OUTPUT);
  pinMode(CE3, OUTPUT);
  digitalWrite(CE, HIGH);
  digitalWrite(CE3, HIGH);
  pinMode(safePin, INPUT);
  pinMode(safeLed, OUTPUT);
  pinMode(dataLed, OUTPUT);
  pinMode(masterOrder, INPUT);
  radio.begin();
  //digitalWrite(8,HIGH);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  //radio.startListening();
  //digitalWrite(rf24SafePin, LOW);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  delay(20000);
  sensor();
}

void loop() {
 // Serial.println(intervalSensor);
  if (!digitalRead(safePin)) {
	//radio.startListening();
	if (setupSPI==false) {
		pinMode(MISO,OUTPUT);
		pinMode(MOSI,OUTPUT);
		pinMode(SCK,OUTPUT);
		digitalWrite(safeLed,LOW);
		SPI.begin();
		setupSPI=true;
	}
  //	SPI.setClockDivider(SPI_CLOCK_DIV2);
	//digitalWrite(rf24SafePin, HIGH);
	//delay(100);
	radio.startListening();
	delay(25);
	if (pohyb==true and counterPohyb>0) {
		counterPohyb--;
	} else {
		counterPohyb=1000;
		pohyb=false;
	}
	int cidla[2];
	if (radio.available()) {
		radio.read(&cidla, sizeof(cidla));
		if (cidla[0]==1) {
			pohyb = true;
			digitalWrite(showLED2, HIGH);
		} else if (cidla[0]==0 and pohyb==false) {
			digitalWrite(showLED2, LOW);
		}
		if (cidla[1]==1) {
			digitalWrite(showLED3, HIGH);
		} else if (cidla[1]==0) {
			digitalWrite(showLED3, LOW);
		}
		if (cidla[0]==2) {
			Serial.println("I02");
		}
		if (cidla[1]==2) {
			Serial.println("I12");
		}
		
		radio.stopListening();
		delay(25);
		//digitalWrite(rf24SafePin,LOW);
		//delay(100);
	}
  //	SPI.setClockDivider(SPI_CLOCK_DIV8);
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= intervalSensor) {
		SPI.begin();
		SPI.setClockDivider(SPI_CLOCK_DIV8);
		delay(1000);
		sensor();
		previousMillis = currentMillis;
	}
	if (digitalRead(masterOrder)) {
		SPI.begin();
		SPI.setClockDivider(SPI_CLOCK_DIV8);
  		//digitalWrite(showLED1, HIGH);
  		//digitalWrite(showLED2, HIGH);
		digitalWrite(dataLed, HIGH);
		digitalWrite(CE3, LOW);
		data = SPI.transfer(1);
		prevedeniDatNaKlavesu(data);
		//delay(100);
		odeslaniDatSensoru();
		//delay(150);
		digitalWrite(CE3,HIGH);
		digitalWrite(dataLed, LOW);
		if (isDigit(klavesa)) {
			Serial.print('I');
			Serial.println(klavesa);
		} else {	
			Serial.println(klavesa);
		}
		delay(250);
	}
	radio.startListening();
  } else {
	setupSPI=false;
	//SPI.endTransaction();
	SPI.end();
	pinMode(MISO, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(SCK, INPUT);
	digitalWrite(safeLed, HIGH);
	delay(1000);
  }
  if (Serial.available()) {
	if (millis()+halfHour - halfHourWait >= halfHour) {
		halfHourWait = millis()+halfHour;
		int i = Serial.read();
		Serial.println(tep);
		Serial.println(vlh);
		Serial.println(tlak);
		Serial.println(uhlikPPM);
		Serial.println(methanPPM);
		Serial.println(CO2);
	}
  }
}
