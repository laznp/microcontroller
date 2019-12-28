#define TOUCH_PIN D0
#define LED_OUT D5
#define LED_IN D3
int touchVal = 0;
int ledState = 0;
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED_OUT, OUTPUT);
  pinMode(LED_IN, INPUT);
  Serial.println();
  Serial.println();
}

void loop() {
  touchVal = digitalRead(TOUCH_PIN);
  ledState = digitalRead(LED_IN);
  if(touchVal == HIGH && ledState == HIGH) {
    digitalWrite(LED_OUT, LOW);
    Serial.println("touch low");
  } else if(touchVal == HIGH && ledState == LOW) {
    digitalWrite(LED_OUT, HIGH);
    Serial.println("touch high");
  }
  delay(500);
}
