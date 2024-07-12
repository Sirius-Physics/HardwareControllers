#include "Arduino.h"
#include "utils.h"

const int digitalOutputPin2 = 10, digitalInputPin1 = 11;
double lastDistance = 0.0, lastMRUATime = 0.0, lastSpeed = 0.0;

void MRUAConfig() {
  pinMode(digitalOutputPin2, OUTPUT);  // Configura el pin del trigger como salida
  pinMode(digitalInputPin1, INPUT); // Configura el pin del echo como entrada
} 

void readDistance() {
  digitalWrite(digitalOutputPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(digitalOutputPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(digitalInputPin1, LOW);
  unsigned long duration = pulseIn(digitalInputPin1, HIGH);
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