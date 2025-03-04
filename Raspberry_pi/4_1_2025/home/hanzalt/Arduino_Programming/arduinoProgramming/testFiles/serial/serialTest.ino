
void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.println("Hello world!");
	delay(1000);
//	if (Serial.available()){
//		int message = Serial.read();
//		Serial.print("Character received is: ");
//		Serial.println(message, DEC);
//	}
}
