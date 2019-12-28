#define LDR_IN D0
#define LED_OUT D5
void setup() {
	Serial.begin(115200);
	pinMode(LDR_IN, INPUT);
	pinMode(LED_OUT, OUTPUT);
	digitalWrite(LED_OUT, LOW);
}

void loop() {
	if (analogRead(LDR_IN) > 0) {
		digitalWrite(LED_OUT, HIGH);
	} else {
		digitalWrite(LED_OUT, LOW);
	}
	Serial.println(luxVal);
	delay(1000);

}