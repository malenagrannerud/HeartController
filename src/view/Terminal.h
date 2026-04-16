#ifndef TERMINAL_H
#define TERMINAL_H

void setupTerminal();
void restoreTerminal();
bool keyPressed();
char getKey();
void printHeader();

void printDataRow(float time, float hr, 
                  float rap, float coRV, float pap, float rpmRight,
                  float lap, float coLV, float aop, float rpmLeft, 
                  float balance,
                  const char* alarm);

void printSummary(int count, float avgHR, float avgCO, float avgBalance,
                  float avgRAP, float avgPAP, float avgLAP, float avgAoP,
                  float minRAP, float maxRAP, float minPAP, float maxPAP,
                  float minLAP, float maxLAP, float minAoP, float maxAoP,
                  float avgErrorRight, float avgErrorLeft);

void printPulseMessage(float hr);
char waitForStart();

#endif