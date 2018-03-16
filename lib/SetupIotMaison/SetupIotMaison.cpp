#include "SetupIotMaison.h"
#include <LocalConfig.h>
#include <HardwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// clients & config
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
LocalConfig localConfig;

/*
 * Connexion au réseau WiFi
 */
void setupWiFi() {
  localConfig = getLocalConfig();

  Serial.println("Connexion WiFi en cours");

  WiFi.begin(localConfig.wifiSSID, localConfig.wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connexion WiFi etablie ");
  Serial.print("=> Addresse IP : ");
  Serial.print(WiFi.localIP());
}


/*
 * Reconnexion à MQTT
 */
void reconnectMQTT() {
  //Boucle jusqu'à obtenur une reconnexion
  while (!mqttClient.connected()) {
    Serial.println("Connexion au serveur MQTT...");
    if (mqttClient.connect("ESP8266Client", localConfig.mqttUser, localConfig.mqttPassword)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, erreur : ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

/*
 * setup de la connexion à MQTT
 */
void setupMQTT() {
  mqttClient.setServer(localConfig.mqttServer, 1883);
}
