#include "sound_sensor.h"
#include <arduinoFFT.h>

#define SAMPLES 128
#define SAMPLING_FREQUENCY 4000
#define THRESHOLD 1000

static int soundSensorPin = 35;  // default pin; can be updated

// Declare real and imaginary arrays
double vReal[SAMPLES];
double vImag[SAMPLES];

// Properly instantiate the ArduinoFFT object with <double> and required parameters
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

void setupSoundSensor(int pin) {
  soundSensorPin = pin;
  Serial.println("Sound Sensor Initialized for Cry Detection");
}

bool isBabyCrying() {
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(soundSensorPin);
    vImag[i] = 0;
    delayMicroseconds(1000000 / SAMPLING_FREQUENCY);
  }

  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  for (int i = 0; i < SAMPLES / 2; i++) {
    double freq = (i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES;
    if (freq >= 300 && freq <= 3000 && vReal[i] > THRESHOLD) {
      return true;
    }
  }

  return false;
}
