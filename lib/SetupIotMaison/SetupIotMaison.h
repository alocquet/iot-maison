#ifndef SETUP_IOT_MAISON_H_   /* Include guard */
#define SETUP_IOT_MAISON_H_

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LocalConfig.h>

// clients & config
extern WiFiClient wifiClient;
extern PubSubClient mqttClient;
extern LocalConfig localConfig;

// setup functions
void setupWiFi();
void setupMQTT();
void reconnectMQTT();

#endif 