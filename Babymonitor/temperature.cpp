#include "temperature.h"
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
void setupTemperatureSensor() {
  dht.begin();
}
float readTemperature() {
  return dht.readTemperature();
}
float readHumidity() {
  return dht.readHumidity();
}
