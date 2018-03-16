// -------------- Libraries --------------
#include <LocalConfig.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

LocalConfig currentConf;

void printMenu();
void checkWifi();

void setup() {
  EEPROM.begin(512);
  Serial.begin(9600);

  currentConf = getLocalConfig();
  printMenu();
}

void readValue(char result[], int size) {
  while(!Serial.available()) {
    delay(100);
  }
  for(int i=0; i < size ; i++){
     if(Serial.available()){
       result[i] = Serial.read();
     } else {
       result[i] = '\0';
       break;
     }
  }
}

void loop() {
  if(Serial.available()){
    onMenuSelection(Serial.read());
  }
}
