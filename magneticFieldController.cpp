#include <Adafruit_VL53L0X.h>
#include <ArduinoJson.h>
#include "utils.h"

const int analogPin1 = A0;
const float sensitivity = 0.0014, offsetVoltage = 2.5;
int rawValue = 400;
float magneticFieldReference;
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// MAGNETIC FIELD
double readMmDistance(){
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // Pass in 'true' to get debug data printout
  return (measure.RangeStatus != 4) ? measure.RangeMilliMeter : 0;
}

void mmDistanceSensorCalibration(){
  if (!lox.begin()) {
    Serial.println("ERROR: VL53L0X SENSOR");
  }
}

void hallSensorCalibration(){
  int numMeasurements = 120; 
  // Takes several initial measurements and calculates the average to use as a reference
  float magneticFieldSum = 0;
  for(int i=0; i<numMeasurements; i++) {
    float voltage = analogRead(analogPin1) * (5.0 / 1023.0) - offsetVoltage;
    magneticFieldSum += voltage / sensitivity;
    delay(10); // Small pause between readings
  }
  magneticFieldReference = magneticFieldSum / numMeasurements;
}

void magneticFieldConfig() {
  mmDistanceSensorCalibration();
  hallSensorCalibration();
};

void readMagneticField(){
  rawValue = analogRead(analogPin1);
  float outputVoltage = rawValue * (5.0 / 1023.0) - offsetVoltage;
  float magneticField = outputVoltage / sensitivity - magneticFieldReference;
  magneticField = abs(round(magneticField * 100.0) / 100.0);

  double distance = readMmDistance();
  unsigned long currentTime = getCurrentTime();

  createAndSerializeJson(
    "MF", 
    "field", magneticField,
    "distance", distance,
    "time", currentTime
  );
}