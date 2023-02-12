#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
// Add your MQTT Broker IP address/Username/Password combination
const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";
const char* brokerUser = "REPLACE_WITH_YOUR_BROKER_USERNAME";
const char* brokerPass = "REPLACE_WITH_YOUR_BROKER_PASSWORD";
// Add your MQTT Broker Topic
const char* Topic = "REPLACE_WITH_YOUR_TOPIC";

/*
// If you need two different topics add your MQTT Broker in&out Topic combination
const char* inTopic = "REPLACE_WITH_YOUR_INTOPIC";
const char* outTopic = "REPLACE_WITH_YOUR_OUTTOPIC";
*/

WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char messages [50];

// LED Pin
#define ledPin 4;

void setup() {
  Serial.begin(115200);
  // default settings
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(100);
  // We start by connecting to a WiFi network
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", String(brokerUser), String(brokerPass))) {
      Serial.println("\nConnected to: ");
      Serial.println(mqtt_server);
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageLed;

  for(int i=0; i<length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
  if (String(topic) == String(Topic)) {
    Serial.print("Changing output to ");
    if(messageLed == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageLed == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  currentTime = millis();
  if(currentTime - lastTime > 4000){
     count++;
     snprintf(messages, 75, "Count: %ld", count);
     Serial.print("Sending messages: ");
     Serial.println(messages);
     client.publish(outTopic, messages);
     lastTime = millis();

  }
}
