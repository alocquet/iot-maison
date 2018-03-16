#include <local_config.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// which analog pin to connect
#define THERMISTORPIN A0
// resistance at 20 degrees C
#define THERMISTORNOMINAL 11200      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 20
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3850
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    

#define temperature_topic "sensor/line/temperature"  //Topic température

//Buffer qui permet de décoder les messages MQTT reçus
char message_buff[100];
LocalConfig localConfig;

WiFiClient espClient;
PubSubClient client(espClient);

void setup(void) {
  EEPROM.begin(512);
  Serial.begin(9600);

  localConfig = getLocalConfig();
  
  setup_wifi();           //On se connecte au réseau wifi
  client.setServer(localConfig.mqttServer, 1883);    //Configuration de la connexion au serveur MQTT
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

  uint8_t i;
  float a0_value;
  float resistor;
  
  // read value
  a0_value = analogRead(0);
  
  // convert the value to resistance
  resistor = SERIESRESISTOR * (1023 / a0_value - 1);
  // Serial.print("Thermistor resistance "); 
  // Serial.println(resistor);
 
  float steinhart;
  steinhart = resistor / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
   
  client.publish(temperature_topic, String(steinhart).c_str(), true);   //Publie la température sur le topic temperature_topic

}
