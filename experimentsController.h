#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

void siriusInitConfig();
void lcdStartupConfig();
void lcdShow(int x, int y, String text, bool clearState);
void detectExperiment();
void handleExperimentSelection();

#endif