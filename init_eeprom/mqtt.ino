#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void checkMQTT() {
  Serial.println(" * MQTT connection test...");

  WiFiClient espClient;
  PubSubClient client(espClient);
  client.setServer(currentConf.mqttServer, 1883);

  if (client.connect("ESP8266Client", currentConf.mqttUser, currentConf.mqttPassword)) {
    Serial.println("\nMQTT is properly configured !");
  } else {
    Serial.println("\n/!\\ MQTT is not properly configured :(");
  }

}

void readMQTTServer() {
  Serial.println("MQTT Server?");
  readValue(currentConf.mqttServer, 16);
}

void readMQTTUser() {
  Serial.println("MQTT User?");
  readValue(currentConf.mqttUser, 16);
}

void readMQTTPassword() {
  Serial.println("MQTT Password?");
  readValue(currentConf.mqttPassword, 32);
}

