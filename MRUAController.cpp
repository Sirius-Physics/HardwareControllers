#include "Arduino.h"
#include "pinsController.h"
#include "utils.h"

double lastDistance = 0.0, lastMRUATime = 0.0, lastSpeed = 0.0;

void readDistance() {
  digitalWrite(DIGITAL_OUTPUT_PIN_2, LOW);
  delayMicroseconds(2);
  digitalWrite(DIGITAL_OUTPUT_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(DIGITAL_INPUT_PIN_1, LOW);
  unsigned long duration = pulseIn(DIGITAL_INPUT_PIN_1, HIGH);
  double distance = (duration * 0.0343) / 2;

  if (distance > 65.0 || distance < 2.0){
    distance = 0.0;
  } else if (abs(distance - lastDistance) < 0.30){
    distance = lastDistance;
  }

  unsigned long currentTime = getCurrentTime();
  double speed = (distance - lastDistance) / (currentTime - lastMRUATime); // Speed in cm/s
  double acceleration = (speed - lastSpeed) / (currentTime - lastMRUATime); // Acceleration in cm/s^2
  lastSpeed = speed;
  if(distance != lastDistance){
    lastDistance = distance;
  }
  createAndSerializeJson(
    "MRUA", 
    "time", currentTime, 
    "distance", distance, 
    "speed", speed, 
    "acceleration", acceleration
  );
}