#include <Arduino.h>
#include "pinsController.h"

void setPinsConfig() {
  pinMode(DIGITAL_INPUT_PIN_1, INPUT);
  pinMode(DIGITAL_INPUT_PIN_2, INPUT);
  pinMode(DIGITAL_INPUT_PIN_3, INPUT);

  pinMode(DIGITAL_OUTPUT_PIN_1, OUTPUT);
  pinMode(DIGITAL_OUTPUT_PIN_2, OUTPUT);
} 