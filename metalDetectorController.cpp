#include <Adafruit_VL53L0X.h>
#include <FreqCount.h>
#include "pinsController.h"
#include "utils.h"

int baseFrecuency = 0, frecuencyMD = 0, sensivityMD = 2;

void generateTone(int pin, int duration, int repetitions){
  for(int i=0; i<repetitions; i++) {
    digitalWrite(pin, HIGH);
    delay(duration);
    digitalWrite(pin, LOW);
    delay(duration);
  }
}

void detectMetal() {
  frecuencyMD = FreqCount.read();
  int difference = baseFrecuency - frecuencyMD;
  // Ferrous metal
  if(difference > sensivityMD){
    generateTone(DIGITAL_OUTPUT_PIN_1, 2, 10);
    delay(40-(constrain(difference*5,10,40)));
    createAndSerializeJson(
      "MD", 
      "isFerrous", "1"
    );
  }
  // Non-Ferrous metal
  else if(difference <- sensivityMD){
    difference = -difference;
    generateTone(DIGITAL_OUTPUT_PIN_1, 1, 20);
    delay(40-(constrain(difference * 5, 10, 40)));
    createAndSerializeJson(
      "MD", 
      "isFerrous", "0"
    );
  }
  frecuencyMD = 0;
}

void setupMD(){
  FreqCount.begin(200);
  frecuencyMD = FreqCount.read();
  baseFrecuency = frecuencyMD;

  for(int i=0; i<5; i++){
    generateTone(DIGITAL_OUTPUT_PIN_1, 2, 10);
    delay(20);
    frecuencyMD = FreqCount.read();
    if(frecuencyMD != baseFrecuency){
      baseFrecuency = frecuencyMD;
      i = 0;
    }
  }
  generateTone(DIGITAL_OUTPUT_PIN_1, 1, 20);
}