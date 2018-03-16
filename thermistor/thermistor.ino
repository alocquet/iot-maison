#include <EEPROM.h>
#include <PubSubClient.h>
#include <SetupIotMaison.h>

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

void setup(void) {
  EEPROM.begin(512);
  Serial.begin(9600);

  setupWiFi();
  setupMQTT();
}

void loop() {
  delay(10000);

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  // read value
  float temperature = analogValueToTemperature(analogRead(0));

  Serial.print("Temperature ");
  Serial.print(temperature);
  Serial.println(" °C");

  mqttClient.publish(temperature_topic, String(temperature).c_str(), true);   //Publie la température sur le topic temperature_topic
}

/**
   compute temperature in Celcius from analog value.
*/
float analogValueToTemperature(float value) {
  float resistor = SERIESRESISTOR * (1023 / value - 1);
  float result;
  result = resistor / THERMISTORNOMINAL;     // (R/Ro)
  result = log(result);                  // ln(R/Ro)
  result /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  result += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  result = 1.0 / result;                 // Invert
  result -= 273.15;                         // convert to C
  return result;
}

