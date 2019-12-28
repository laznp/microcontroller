#define MIC_PIN D0
#define LED_OUT D5

void setup() {
	Serial.begin(115200);
	pinMode(MIC_PIN, INPUT);
	pinMode(LED_OUT, OUTPUT);
}

void loop() {
	if (digitalRead(MIC_PIN) == HIGH) {
		digitalWrite(LED_OUT, HIGH);
	} else {
		digitalWrite(LED_OUT, LOW);
	}
}