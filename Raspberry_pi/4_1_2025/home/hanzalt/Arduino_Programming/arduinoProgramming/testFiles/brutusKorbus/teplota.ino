// Teploměr a vlhkoměr DHT11/22

// připojení knihovny DHT
#include "DHT.h"
// nastavení čísla pinu s připojeným DHT senzorem
#define pinDHT 5

// odkomentování správného typu čidla
//#define typDHT11 DHT11     // DHT 11
#define typDHT22 DHT22   // DHT 22 (AM2302)

// inicializace DHT senzoru s nastaveným pinem a typem senzoru
DHT mojeDHT(pinDHT, typDHT22);

int led1 = 8;
int led2 = 7;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // zapnutí komunikace
  mojeDHT.begin();
}

void loop() {
  // pomocí funkcí readTemperature a readHumidity načteme
  // do proměnných tep a vlh informace o teplotě a vlhkosti,
  // čtení trvá cca 250 ms
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  Serial.print("teplota: ");
  Serial.print(tep);
  Serial.println("C");
  Serial.print("vlhkost: ");
  Serial.print(vlh);
  Serial.println("%");
  if (tep>=27.5) {
	digitalWrite(led1,HIGH);
  } else {
	digitalWrite(led1,LOW);
  }
  if (vlh>=65) {
	digitalWrite(led2,HIGH);
  } else {
	digitalWrite(led2,LOW);
  }
  delay(100);
}
