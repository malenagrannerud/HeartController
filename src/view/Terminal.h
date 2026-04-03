#ifndef TERMINAL_H
#define TERMINAL_H

void setupTerminal();
void restoreTerminal();
bool keyPressed();
char getKey();
void printHeader();
void printDataRow(float time, float hr,
                  float rap, float papTarget, float papActual, 
                  float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, 
                  float rpmLeft, float vLeft, float errorLeft);
void printSummary(int count, float avgHR, float avgRAP, float avgLAP,
                  float avgPAP, float avgAoP, float avgErrorRight, float avgErrorLeft,
                  float minRAP, float maxRAP, float minLAP, float maxLAP,
                  float minPAP, float maxPAP, float minAoP, float maxAoP);
void printPulseMessage(float hr);
char waitForStart();

#endif