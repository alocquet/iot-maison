#include <LocalConfig.h>
#include <EEPROM.h>
#include <HardwareSerial.h>

LocalConfig getLocalConfig() {
  LocalConfig config; //Variable to store custom object read from EEPROM.
  EEPROM.get(0, config);
  return config;
}

void writeLocalConfig(LocalConfig conf) {
  EEPROM.put(0, conf);
  EEPROM.commit();
}

void printLocalConfig(LocalConfig conf){
  Serial.print(" » WiFi SSID:");
  Serial.println(conf.wifiSSID);
  Serial.print(" » WiFi Password: ");
  Serial.println(conf.wifiPassword);
  Serial.print(" » MQTT Server: ");
  Serial.println(conf.mqttServer);
  Serial.print(" » MQTT User: ");
  Serial.println(conf.mqttUser);
  Serial.print(" » MQTT Password: ");
  Serial.println(conf.mqttPassword);
}

