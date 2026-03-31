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
void printSummary(int count);
char waitForStart();
bool handleUserInput(float& hr, bool& running);
void printPulseMessage(float hr);

#endif 