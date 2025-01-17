#include <Arduino.h>
#include "utils.h"

const int analogPin1 = A0;

double thermister(int RawADC) {
  double Temp;
  Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;
  return Temp;
}

void readTemperature() {
  int val = analogRead(analogPin1);
  int temp = thermister(val);
  unsigned long currentTime = getCurrentTime();
  createAndSerializeJson(
    "TMT", 
    "temperature", temp, 
    "time", currentTime
  );
}