#include <Arduino.h>
#include "pinsController.h"
#include "utils.h"

const unsigned long ktInterval = 1000000;
int nodeValue, lastValue = 0;
unsigned long ktFrecuency = 0, ktLastTime = 0;

void kundtsTubeConfig() {
  ktLastTime = micros();
}

void soundSensorCalibration(){
  int numMeasurements = 120, frecuencySum = 0;
  // Takes several initial measurements and calculates the average to use as a reference
  for(int i=0; i<numMeasurements; i++) {
    frecuencySum += analogRead(ANALOG_PIN_1);
    delay(10); // Small pause between readings
  }
  nodeValue = frecuencySum / numMeasurements; // Average value
}

void readFrecuencyLevels(){
  int currentValue = analogRead(ANALOG_PIN_1);
  unsigned long currentTimeKD = micros();

  // Detect a node crossing
  if((currentValue < nodeValue && lastValue >= nodeValue) || (lastValue < nodeValue && currentValue >= nodeValue)) {
    ktFrecuency++;
  }
  lastValue = currentValue;

  if(currentTimeKD - ktLastTime >= ktInterval) {
    ktFrecuency /= 2; // Divide by 2 since two node crossings are detected
    createAndSerializeJson(
      "KD", 
      "frecuency", ktFrecuency, 
      "amplitude", currentValue
    );
    ktFrecuency = 0;
    ktLastTime = currentTimeKD;
  }
}