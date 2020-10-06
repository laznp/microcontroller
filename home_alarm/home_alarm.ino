#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#define LDR_IN D0
#define BUZZER_PIN D3
#define FREQ 1000

const char* ssid = "wifi_ssid";
const char* pswd = "wifi_password";
const char* mqttServer = "mqtt_server";
const char* alertHost = "alert_host_domain";
const int mqttPort = 1883;
const char* mqttUser = "mqtt_user";
const char* mqttPassword = "mqtt_password";
const char* pubSubTopic = "mqtt_topic";

int wifiStatus;
WiFiClientSecure httpsClient;
HTTPClient http;
WiFiClient espClient;
PubSubClient client(espClient);
bool alarmStatus = false;

void setup() {
  Serial.begin(115200);
  pinMode(LDR_IN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  delay(200);

  // WiFi Connection
  Serial.println();
  Serial.println();
  Serial.print("You are connecting to : ");
  Serial.println(ssid);
  WiFi.begin(ssid, pswd);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  wifiStatus = WiFi.status();
  if(wifiStatus == WL_CONNECTED) {
    Serial.println("");
    Serial.print("Connected to : "); 
    Serial.println(ssid);
    Serial.print("Your IP address is : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("WiFi not connected!");
  }

  // MQTT Connection
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.print("Connected to : mqtt://");
      Serial.println(mqttServer);
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(pubSubTopic);
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String message = (char*)payload;
  if (message == "off") {
    ESP.restart();
  }
}

void send_alert() {
  httpsClient.setInsecure();
  httpsClient.connect(alertHost, 443);
  http.begin(httpsClient, alertHost);
  http.addHeader("X-Device-ID","ESP8266");
  int httpCode = http.GET();
  String httpReturn = http.getString();
  Serial.print("HTTP Code : ");
  Serial.println(httpCode);
  Serial.print("HTTP Return : ");
  Serial.println(httpReturn);
  http.end();
}

void loop() {
  int sensorValue = digitalRead(D0);
  if (sensorValue != 0) {
    Serial.println("Sending Alert...");
    alarmStatus = true;
    send_alert();
    while (alarmStatus) {
      client.loop();
      if(!alarmStatus) {
        noTone(BUZZER_PIN);
        sensorValue = 0;
        break;
      } else {
        tone(BUZZER_PIN, FREQ);
        delay(500);
        noTone(BUZZER_PIN);
        delay(100);
      }
    }
  }
}
