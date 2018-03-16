#ifndef LOCAL_CONFIG_H_   /* Include guard */
#define LOCAL_CONFIG_H_

typedef struct {
  char wifiSSID[16];
  char wifiPassword[32];
  char mqttServer[16];
  char mqttUser[16];
  char mqttPassword[32];
} LocalConfig;

LocalConfig getLocalConfig();
void writeLocalConfig(LocalConfig conf);
void printLocalConfig(LocalConfig conf);

#endif 
