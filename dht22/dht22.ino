#include <SetupIotMaison.h>
#include <EEPROM.h>
#include <DHT.h>

#define temperature_topic "sensor/line/temperature"  //Topic température
#define humidity_topic "sensor/line/humidity"        //Topic humidité

#define DHTPIN D4    // Pin sur lequel est branché le DHT
#define DHTTYPE DHT22         // DHT 22  (AM2302)

// wrapper DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  EEPROM.begin(512);
  Serial.begin(9600);

  setupWiFi();
  setupMQTT();
  dht.begin();
}

void loop() {
  delay(10000);

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  long now = millis();

  //Lecture de l'humidité ambiante
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();

  //Inutile d'aller plus loin si le capteur ne renvoi rien
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print(" | Humidite : ");
  Serial.println(h);

  if ( isnan(t) || isnan(h)) {
    Serial.println("Echec de lecture ! Verifiez votre capteur DHT");
    return;
  }

  mqttClient.publish(temperature_topic, String(t).c_str(), true);   //Publie la température sur le topic temperature_topic
  mqttClient.publish(humidity_topic, String(h).c_str(), true);      //Et l'humidité

}

