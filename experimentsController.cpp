#include <Arduino.h>
#include "utils.h"
#include "pinsController.h"
#include "interfaceController.h"
#include "MRUAController.h"
#include "magneticFieldController.h"
#include "metalDetectorController.h"
#include "kundtsTubeController.h"
#include "thermometerController.h"
#include "freeFallController.h"

unsigned long lastPrintTime = 0;
String experimentDetector, check = "", serialReceivedData = "";   // Checking controllers
bool freqStatus, loopState = true, startupState = false, lastPrintState = true;          // Status controllers

enum Experiment {
  NONE,
  MRUA,
  FF,
  MF,
  MD,
  KD,
  TMT
};

Experiment getExperiment(const String& str) {
  if (str == "MRUA") return MRUA;
  if (str == "FF") return FF;
  if (str == "MF") return MF;
  if (str == "MD") return MD;
  if (str == "KD") return KD;
  if (str == "TMT") return TMT;
  return NONE;
}

void siriusInitConfig(){
  // FREE FALL
  freeFallConfig();
  // KUNDT'S TUBE
  kundtsTubeConfig();
  // SYSTEM
  setPinsConfig();
  lcdStartupConfig();
  Serial.println("SIRIUS STARTED"); // Software signal SIRIUS system inicialization
}

void detectExperiment() {
  // Check the experience to proceed
  while (loopState == true){  
    if (Serial.available() > 0){ 
      check = Serial.readStringUntil('\n');
      check.trim();
      Experiment exp = getExperiment(check);
      switch (exp) {
        case MRUA:
            Serial.println("MRUA");
            loopState = false;
            experimentDetector = "MRUA";
            break;
        case FF:
            Serial.println("FF");
            loopState = false;
            experimentDetector = "FF";
            break;
        case MF:
            Serial.println("MF");
            loopState = false;
            experimentDetector = "MF";
            break;
        case MD:
            Serial.println("MD");
            loopState = false;
            experimentDetector = "MD";
            break;
        case KD:
            Serial.println("KD");
            loopState = false;
            experimentDetector = "KD";
            break;
        case TMT:
            Serial.println("TMT");
            loopState = false;
            experimentDetector = "TMT";
            break;
        default:
            break;
      }
    }
  }
}

void executeOperation(void (*experimentFn)(), int delayTime){
  if (Serial.available() > 0) {
    serialReceivedData = Serial.readStringUntil('\n');
    serialReceivedData.trim();
    if (serialReceivedData == "INIT"){
      lastPrintState = true;
      saveInitialTime();      
      while (lastPrintState == true){ 
        if ((millis() - lastPrintTime) > delayTime) {
          experimentFn();
          lastPrintTime = millis();
        }
        if (Serial.available() > 0){
          serialReceivedData = Serial.readStringUntil('\n');
          serialReceivedData.trim();
          if (serialReceivedData == "PAUSE"){
            lastPrintState = false;
            break;
          } else if (serialReceivedData == "ESC"){
            startupState = false;
            lastPrintState = false;
            experimentDetector = "null";
            loopState = true;
            lcdHomeScreen();
            break;
          }
        }    
      }
    } else if (serialReceivedData == "ESC"){
      startupState = false;
      experimentDetector = "null";
      loopState = true;
      lcdHomeScreen();
    } 
  }
}

void handleExperimentSelection() {
  Experiment currentExp = getExperiment(experimentDetector);
  switch (currentExp) {
    case MRUA:
        while (experimentDetector == "MRUA") {
          if (startupState == false){
            lcdShow(6, 1, "Uniform", true);
            lcdShow(0, 2, "Rectilinear Movement", false);
            startupState = true;
          }
          executeOperation(readDistance, 250);
        }
        break;
    case FF:
        while (experimentDetector == "FF") {
          if (startupState == false){
            lcdShow(6, 1, "Freefall", true);
            startupState = true;
          }
          executeOperation(calculateGravityAcceleration, 10);
        }
        break;
    case MF:
        while (experimentDetector == "MF") {
          if (startupState == false){
            magneticFieldConfig();
            lcdShow(3, 1, "Magnetic Field", true);
            startupState = true;
          }
          executeOperation(readMagneticField, 250);
        }
        break;
    case MD:
        while (experimentDetector == "MD") {
          if (startupState == false){
            setupMD();
            lcdShow(3, 1, "Metal Detector", true);
            freqStatus = true;
          }
          executeOperation(detectMetal, 250);
        }
        break;
    case KD:
        while (experimentDetector == "KD") {
          if (startupState == false){
            soundSensorCalibration();
            lcdShow(4, 1, "Kundts Tube", true);
            startupState = true;
          }
          executeOperation(readFrecuencyLevels, 0);
        }
        break;
    case TMT:
        while (experimentDetector == "TMT") {
          if (startupState == false){
            lcdShow(5, 1, "Termometer", true);
            startupState = true;
          }
          executeOperation(readTemperature, 250);
        }
        break;
    default:
        break;
  }
}