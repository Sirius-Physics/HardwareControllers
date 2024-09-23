#include "Arduino.h"
#include "utils.h"

const int digitalOutputPin2 = 10, digitalInputPin1 = 11;
const int analogPinSeed = A0;  // Pin analógico desconectado
double lastDistance = 0.0, lastMRUATime = 0.0, lastSpeed = 0.0;

void MRUAConfig() {
  pinMode(digitalOutputPin2, OUTPUT);  // Configura el pin del trigger como salida (aunque no se usará)
  pinMode(digitalInputPin1, INPUT);    // Configura el pin del echo como entrada (aunque no se usará)
  randomSeed(analogRead(analogPinSeed));  // Inicializa el generador de números aleatorios con un valor del pin analógico desconectado
} 

void readDistance() {
  // Simula una medición de distancia aleatoria entre 2.0 y 65.0 cm
  double distance = random(20, 650) / 10.0;  // Genera un número aleatorio entre 2.0 y 65.0 cm

  // Si la diferencia entre la nueva distancia y la anterior es pequeña, mantenemos la anterior
  if (abs(distance - lastDistance) < 0.30) {
    distance = lastDistance;
  }

  unsigned long currentTime = getCurrentTime();
  double speed = (distance - lastDistance) / (currentTime - lastMRUATime);  // Velocidad en cm/s
  double acceleration = (speed - lastSpeed) / (currentTime - lastMRUATime); // Aceleración en cm/s^2
  lastSpeed = speed;

  if (distance != lastDistance) {
    lastDistance = distance;
  }

  // Serializa los valores en JSON
  createAndSerializeJson(
    "MRUA", 
    "time", currentTime, 
    "distance", distance, 
    "speed", speed, 
    "acceleration", acceleration
  );
}
