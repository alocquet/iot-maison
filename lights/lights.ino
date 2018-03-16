#include <EEPROM.h>
#include <PubSubClient.h>
#include <SetupIotMaison.h>

#define LIGHT_TOPIC "switch/line/light"
#define LIGHT_PIN D4

void setup() {
  EEPROM.begin(512);
  Serial.begin(9600);     //Facultatif pour le debug

  setupWiFi();
  setupMQTT();
  setupLightSwitch();
}

void setupLightSwitch() {
  pinMode(LIGHT_PIN, OUTPUT);
  mqttClient.setCallback(switchLight);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
    mqttClient.subscribe(LIGHT_TOPIC);
  }
  mqttClient.loop();
}

//Buffer used to store received MQTT messages
char message_buff[4];

/**
 * Callback used to switch light when a message is received on MQTT topic
 */
void switchLight(char* topic, byte* payload, unsigned int length) {
  int i = 0;

  Serial.println("Message recu =>  topic: " + String(topic));
  Serial.print(" | longueur: " + String(length, DEC));
  
  // create character buffer with ending null terminator (string)
  for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);

  if ( msgString == "ON" ) {
    digitalWrite(LIGHT_PIN, HIGH);
  } else {
    digitalWrite(LIGHT_PIN, LOW);
  }
}
