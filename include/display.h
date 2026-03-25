#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

class Display {
public:
    void update(float inlet, float target, float outlet, float rpm, float current, float hr, float time);
    void drawBar(std::string label, float value, float maxVal);
    void printStats(float avgInlet, float avgOutlet, float avgError);
};

#endif