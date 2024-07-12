#ifndef METAL_DETECTOR_H
#define METAL_DETECTOR_H

void metalDetectorConfig();
void generateTone(int pin, int duration, int repetitions);
void detectMetal();
void setupMD();

#endif