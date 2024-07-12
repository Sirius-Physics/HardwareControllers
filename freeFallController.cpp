#include <Arduino.h>
#include "utils.h"

const float distanceBetween = 0.50, localGravity = 9.78;
unsigned long topSensorCounter;
int attemptCounter = 0;
byte lastBottomSensorState, lastTopSensorState;

void freeFallConfig() {
  PCICR |= (1 << PCIE0);            // Enable the PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);          // Set pin 8 to trigger a state change
  PCMSK0 |= (1 << PCINT1);          // Set pin 9 to trigger a state change
}

ISR(PCINT0_vect){
  // Save the current counter value
  unsigned long currentTimeFF = micros();
  
  if(PINB & B00000001) {  // Check if D8 is HIGH
    if(lastBottomSensorState == 0) {                         
      lastBottomSensorState = 1;
      unsigned long timeInterval = currentTimeFF - topSensorCounter;
      attemptCounter += 1;

      double calculatedGravity = 2 * (0.5 - 0.938 * (timeInterval / 1E6)) / ((timeInterval / 1E6) * (timeInterval / 1E6));
      double error = (abs(calculatedGravity - localGravity) / localGravity) * 100.0;
      createAndSerializeJson(
        "FF", 
        "acceleration", calculatedGravity, 
        "error", error, 
        "attempt", attemptCounter
      );
    }
  } else if(lastBottomSensorState == 1) {                      
    lastBottomSensorState = 0;                              
  }

  if(PINB & B00000010) {   // Check if D9 is HIGH
    if(lastTopSensorState == 0) {
      lastTopSensorState = 1;
      topSensorCounter = currentTimeFF;
    }
  } else if(lastTopSensorState == 1) {                                           
    lastTopSensorState = 0;                                                     
  }
}

void calculateGravityAcceleration() {
  asm volatile ("nop"); // Thanks to ISR function, the main FF function do not need a operation
}