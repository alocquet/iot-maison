#include <local_config.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define temperature_topic "sensor/line/temperature"  //Topic température
#define humidity_topic "sensor/line/humidity"        //Topic humidité

//Buffer qui permet de décoder les messages MQTT reçus
char message_buff[100];
LocalConfig localConfig;

long lastMsg = 0;   //Horodatage du dernier message publié sur MQTT
bool debug = true;  //Affiche sur la console si True

#define DHTPIN D4    // Pin sur lequel est branché le DHT
#define DHTTYPE DHT22         // DHT 22  (AM2302)

//Création des objets
DHT dht(DHTPIN, DHTTYPE);     
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  EEPROM.begin(512);
  Serial.begin(9600);     //Facultatif pour le debug
  
  localConfig = getLocalConfig();
  
  setup_wifi();           //On se connecte au réseau wifi
  client.setServer(localConfig.mqttServer, 1883);    //Configuration de la connexion au serveur MQTT
  dht.begin();
}


//Connexion au réseau WiFi
void setup_wifi() {
  delay(10);

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


//Reconnexion
void reconnect() {
  //Boucle jusqu'à obtenur une reconnexion
  while (!client.connected()) {
    Serial.println("Connexion au serveur MQTT...");
    Serial.println(localConfig.mqttUser);
    Serial.println(localConfig.mqttPassword);
    if (client.connect("ESP8266Client", localConfig.mqttUser, localConfig.mqttPassword)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, erreur : ");
      Serial.print(client.state());
      Serial.println(" On attend 5 secondes avant de recommencer");
      delay(5000);
    }
  }
}

void loop() {
  delay(10000);
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  
  //Lecture de l'humidité ambiante
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();

  //Inutile d'aller plus loin si le capteur ne renvoi rien
    if ( debug ) {
      Serial.print("Temperature : ");
      Serial.print(t);
      Serial.print(" | Humidite : ");
      Serial.println(h);
    }  
  
    if ( isnan(t) || isnan(h)) {
      Serial.println("Echec de lecture ! Verifiez votre capteur DHT");
      return;
    }
  
    client.publish(temperature_topic, String(t).c_str(), true);   //Publie la température sur le topic temperature_topic
    client.publish(humidity_topic, String(h).c_str(), true);      //Et l'humidité
  
}

