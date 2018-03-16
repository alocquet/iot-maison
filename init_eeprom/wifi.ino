void checkWifi() {
  Serial.println(" * WiFi connection test...");
  WiFi.disconnect();
  WiFi.begin(currentConf.wifiSSID, currentConf.wifiPassword);
  // wait connection
  int count = 0;
  while (WiFi.status() != WL_CONNECTED && count++ < 20) {
    delay(500);
    Serial.print(".");
  }
  // check status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi is properly configured !");
  } else {
    Serial.println("\n/!\\ WiFi is not properly configured :(");
  }
}

void readWifiSSID() {
  Serial.println("Wifi SSID?");
  readValue(currentConf.wifiSSID, 16);
}

void readWifiPassword() {
  Serial.println("Wifi Password?");
  readValue(currentConf.wifiPassword, 32);
}

