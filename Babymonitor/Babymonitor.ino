// Babymonitor.ino

#include <WiFi.h>
#include <PubSubClient.h>

#include "temperature.h"
#include "heart_rate.h"
#include "oled_display.h"
#include "sound_sensor.h"
#include "display_readings.h"

// WiFi credentials
const char* ssid = "Galaxy M3154D4";
const char* password = "varshu465";

// MQTT Broker
//const char* mqtt_server = "172.23.144.31";  // or use your broker IP
const char* mqtt_topic = "incubator/data";

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect WiFi
void connectWiFi() {
  Serial.print("Connecting to WiFi");

  WiFi.begin(ssid, password);

  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retryCount++;

    if (retryCount > 20) {  // ~10 seconds timeout
      Serial.println("\n Failed to connect to WiFi. Check credentials or signal.");
      return;
    }
  }

  Serial.println("\n WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
// Function to connect MQTT
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("BabyMonitorClient")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);  // optional, if you want to receive
    } else {
      Serial.print("Failed. State: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  setupTemperatureSensor();
  setupHeartRateSensor();
  setupOLED();
  setupSoundSensor(35);

  connectWiFi();
  client.setServer("test.mosquitto.org", 1883);
  connectMQTT();

  Serial.println("Baby Monitor Initialized");
}

void loop() {
  if (!client.connected()) connectMQTT();
  client.loop();

  // Read Temperature & Humidity
  float temp = readTemperature();
  float hum = readHumidity();

  // Read Heart Rate & SpO2
  int32_t spo2;
  int8_t validSpO2;
  int32_t heartRate;
  int8_t validHeartRate;
  readHeartRateAndSpO2(spo2, validSpO2, heartRate, validHeartRate);

  // Read Sound Level
  bool babyCrying = isBabyCrying();

  // Print to Serial Monitor
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" C, ");
  Serial.print("Humidity: "); Serial.print(hum); Serial.print(" %, ");
  Serial.print("HR: "); Serial.print(heartRate); Serial.print(" bpm, ");
  Serial.print("SpO2: "); Serial.print(spo2); Serial.print(" %, ");
  Serial.print("Cry Detected: "); Serial.println(babyCrying ? "Yes" : "No");

  // Display on OLED
  displayReadings(temp, hum, heartRate, spo2, babyCrying);

  // Publish to MQTT
  String payload = "{";
  payload += "\"temperature\":" + String(temp) + ",";
  payload += "\"humidity\":" + String(hum) + ",";
  payload += "\"heartRate\":" + String(heartRate) + ",";
  payload += "\"spo2\":" + String(spo2) + ",";
  payload += "\"babyCrying\":" + String(babyCrying);
  payload += "}";

bool ok = client.publish(mqtt_topic, payload.c_str(),true);
Serial.println(ok ? F("Publish OK") : F("Publish FAILED"));

  delay(2000);
}