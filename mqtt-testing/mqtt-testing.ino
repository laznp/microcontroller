#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED LED_BUILTIN

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";
const char* mqttServer = "mqtt_server";
const int mqttPort = 1883;
const char* mqttUser = "mqtt_user";
const char* mqttPassword = "mqtt_password";
const char* pubSubTopic = "mqtt_topic";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
	Serial.begin(115200);

	WiFi.begin(ssid, password);
	Serial.println("");
	Serial.print("Connecting to WiFi");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print("...");
	}
	Serial.println("");
	Serial.print("Connected to : ");
	Serial.println(ssid);

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

  StaticJsonDocument<128> doc;
  const int capacity = JSON_OBJECT_SIZE(3);
  doc["device"] = "ESP8622";
  doc["data"] = "Connected";
  
  char payload[capacity + 2];
  serializeJson(doc, payload);
  client.publish(pubSubTopic, payload);
	client.subscribe(pubSubTopic);

	pinMode(LED, OUTPUT);
}

void callback(char* topic, byte* payload, unsigned int length) {

	Serial.println("-----------------------");
	Serial.print("Topic   : ");
	Serial.println(topic);

	StaticJsonDocument<256> doc;
	deserializeJson(doc, payload, length);
	String data = doc["data"];

	Serial.print("Message : ");
	serializeJson(doc, Serial);

	if (data == "off") {
		digitalWrite(LED, HIGH);
	} else if (data == "on") {
		digitalWrite(LED, LOW);
	}

	Serial.println();
}

void loop() {
	client.loop();
}
