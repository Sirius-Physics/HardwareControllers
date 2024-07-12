#include "experimentsController.h"

void setup() {
  Serial.begin(9600);
  siriusInitConfig(); // SIRIUS system initialization config
}

void loop() {
  detectExperiment();
  handleExperimentSelection();
}