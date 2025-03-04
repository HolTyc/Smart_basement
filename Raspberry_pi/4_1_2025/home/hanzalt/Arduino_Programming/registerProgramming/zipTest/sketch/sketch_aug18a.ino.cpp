#include <Arduino.h>
#line 1 "/tmp/.arduinoIDE-unsaved2023718-94803-1e0teog.kj3k/sketch_aug18a/sketch_aug18a.ino"
// Teploměr a vlhkoměr DHT11/22

// připojení knihovny DHT
#include "DHT.h"
// nastavení čísla pinu s připojeným DHT senzorem
#define pinDHT 5

// odkomentování správného typu čidla
#define typDHT22 DHT22     // DHT 11
//#define typDHT22 DHT22   // DHT 22 (AM2302)

// inicializace DHT senzoru s nastaveným pinem a typem senzoru
DHT mojeDHT(pinDHT, typDHT22);

#line 15 "/tmp/.arduinoIDE-unsaved2023718-94803-1e0teog.kj3k/sketch_aug18a/sketch_aug18a.ino"
void setup();
#line 23 "/tmp/.arduinoIDE-unsaved2023718-94803-1e0teog.kj3k/sketch_aug18a/sketch_aug18a.ino"
void loop();
#line 15 "/tmp/.arduinoIDE-unsaved2023718-94803-1e0teog.kj3k/sketch_aug18a/sketch_aug18a.ino"
void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  // zapnutí komunikace s teploměrem DHT
  mojeDHT.begin();
}

void loop() {
  // pomocí funkcí readTemperature a readHumidity načteme
  // do proměnných tep a vlh informace o teplotě a vlhkosti,
  // čtení trvá cca 250 ms
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  // kontrola, jestli jsou načtené hodnoty čísla pomocí funkce isnan
  if (tep>=27.5) {
    digitalWrite(8,HIGH);
  } else {
    digitalWrite(8,LOW);
  }
  if (vlh>=58) {
    digitalWrite(7,HIGH);
  } else {
    digitalWrite(7,LOW);
  }
  // pauza pro přehlednější výpis
  delay(10);
}
