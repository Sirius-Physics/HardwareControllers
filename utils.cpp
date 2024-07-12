#include <Arduino.h>

unsigned long initialTime;

void saveInitialTime() {
  initialTime = millis();
}

unsigned long getCurrentTime() {
  return (millis() - initialTime)/1000.0;
}