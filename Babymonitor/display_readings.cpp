#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "display_readings.h"

extern Adafruit_SSD1306 display;

void displayReadings(float temperature, float humidity, int heartRate, int spo2, int soundLevel) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Humidity: "); display.print(humidity); display.println(" %");
  display.print("HR: "); display.print(heartRate); display.println(" bpm");
  display.print("SpO2: "); display.print(spo2); display.println(" %");
  display.print("Sound: "); display.print(soundLevel);

  display.display();
}
