void printMenu() {
  Serial.println("\n\n\n");
  Serial.println("╔════════════════════════════════════╗");
  Serial.println("║     EEPROM Local Config Utility    ║");
  Serial.println("╚════════════════════════════════════╝");
  Serial.println(" 1. Reset config");
  Serial.print(" 2. Update Wifi SSID (");
  Serial.print(currentConf.wifiSSID);
  Serial.println(")");
  Serial.print(" 3. Update Wifi Password (");
  Serial.print(currentConf.wifiPassword);
  Serial.println(")");
  Serial.print(" 4. Update MQTT Server (");
  Serial.print(currentConf.mqttServer);
  Serial.println(")");
  Serial.print(" 5. Update MQTT User (");
  Serial.print(currentConf.mqttUser);
  Serial.println(")");
  Serial.print(" 6. Update MQTT Password (");
  Serial.print(currentConf.mqttPassword);
  Serial.println(")");
  Serial.println(" 7. Check config");
  Serial.println(" 8. Write config");
  Serial.println("══════════════════════════════════════");
}

void onMenuSelection(int idx) {
  switch (idx) {
    case '1' :
      currentConf = getLocalConfig();
      printLocalConfig(currentConf);
      break;
    case '2' : readWifiSSID(); break;
    case '3' : readWifiPassword(); break;
    case '4' : readMQTTServer(); break;
    case '5' : readMQTTUser(); break;
    case '6' : readMQTTPassword(); break;
    case '7' : checkConfig(); break;
    case '8' : writeLocalConfig(currentConf); break;
  }
  printMenu();
}

void checkConfig(){
  checkWifi();
  checkMQTT();
}
