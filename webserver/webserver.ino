#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "wifi_ssid";
const char* password = "wifi_pass";
 
ESP8266WebServer server(80);
 
String page = "";
int LEDPin = 14;
void setup(void){
	page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
	pinMode(LEDPin, OUTPUT);
	digitalWrite(LEDPin, LOW);

	delay(1000);
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	Serial.println("");

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/", [](){
		server.send(200, "text/html", page);
	});

	server.on("/LEDOn", [](){
		server.send(200, "text/html", page);
		digitalWrite(LEDPin, HIGH);
		delay(1000);
	});

	server.on("/LEDOff", [](){
		server.send(200, "text/html", page);
		digitalWrite(LEDPin, LOW);
		delay(1000); 
	});

	server.begin();
	Serial.println("Web server started!");
}

void loop(void){
	server.handleClient();
}
