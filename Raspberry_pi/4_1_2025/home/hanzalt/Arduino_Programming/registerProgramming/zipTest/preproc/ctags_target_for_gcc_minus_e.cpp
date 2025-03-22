# 1 "/tmp/.arduinoIDE-unsaved2023718-94803-1e0teog.kj3k/sketch_aug18a/sketch_aug18a.ino"
// Teploměr a vlhkoměr DHT11/22

// připojení knihovny DHT
# 5 "/tmp/.arduinoIDE-unsaved2023718-94803-1e0teog.kj3k/sketch_aug18a/sketch_aug18a.ino" 2
// nastavení čísla pinu s připojeným DHT senzorem


// odkomentování správného typu čidla

//#define typDHT22 DHT22   // DHT 22 (AM2302)

// inicializace DHT senzoru s nastaveným pinem a typem senzoru
DHT mojeDHT(5, DHT22 /* DHT 11*/);

void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  pinMode(8,0x1);
  pinMode(7,0x1);
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
    digitalWrite(8,0x1);
  } else {
    digitalWrite(8,0x0);
  }
  if (vlh>=58) {
    digitalWrite(7,0x1);
  } else {
    digitalWrite(7,0x0);
  }
  // pauza pro přehlednější výpis
  delay(10);
}
