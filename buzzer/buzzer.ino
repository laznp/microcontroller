#define BUZZER_PIN D0
#define FREQ 1000
void setup() {
	Serial.begin(115200);
	pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
	tone(BUZZER_PIN, FREQ);
	delay(1000);
	noTone(BUZZER_PIN);
	delay(1000);
}