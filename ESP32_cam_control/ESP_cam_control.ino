/*
Replace Antalya2G with your own Wifi connection name
Replace TalGeis2020# with your own Wifi password 
Replace driver.cloudmqtt.com with your own mqtt server
Replace esptest/led with your own mqtt server topic
Replace ffdljaie with your own mqtt server username
Replace cWgKJioSrzPh with your own mqtt server password
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "Antalya2G";
const char* password = "TalGeis2020#";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "driver.cloudmqtt.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  // default settings
  //status = bme.begin();  
  setup_wifi();
  client.setServer(mqtt_server, 18708);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageCam;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageCam += (char)message[i];
  }
  Serial.println();

  // Changes the output state according to the message
  if (String(topic) == "espCam") {
    Serial.print("Changing output to ");
    if(messageCam == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageCam == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client","ffdljaie","cWgKJioSrzPh")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esptest");
      client.subscribe("espCam");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now; 
       
    // Convert the value to a char array
    Serial.println("Cam is ready");
    client.publish("esptest", "Cam is activated");
  }
}
