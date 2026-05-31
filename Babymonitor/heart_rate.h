#include <stdint.h>
#ifndef HEART_RATE_H
#define HEART_RATE_H

void setupHeartRateSensor();
void readHeartRateAndSpO2(int32_t &spo2, int8_t &validSpO2, int32_t &heartRate, int8_t &validHeartRate);


#endif
