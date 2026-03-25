#include "../include/display.h"
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

void Display::update(float inlet, float target, float outlet, float rpm, float current, float hr, float time) {
    cout << "\033[2J\033[1;1H";
    cout << "========================================" << endl;
    cout << "TAH Left Ventricle Simulator" << endl;
    cout << "Frank-Starling Control" << endl;
    cout << "========================================" << endl;
    cout << "Tid: " << time << " s | Puls: " << hr << " bpm" << endl;
    cout << "----------------------------------------" << endl;
    
    drawBar("INLET (Preload)  ", inlet, 40);
    drawBar("OUTLET (Target)  ", target, 40);
    drawBar("OUTLET (Actual)  ", outlet, 40);
    
    float error = target - outlet;
    cout << "\nREGLERFEL: " << error;
    if (abs(error) < 0.5) cout << " [BRA]";
    else if (abs(error) < 2.0) cout << " [OK]";
    else cout << " [DALIGT]";
    cout << endl;
    
    cout << "\nMOTOR: " << rpm << " RPM | " << current << " mA" << endl;
    cout << "Frank-Starling: " << inlet << " mmHg -> " << target << " mmHg" << endl;
    cout << "----------------------------------------" << endl;
}

void Display::drawBar(string label, float value, float maxVal) {
    int len = 40;
    int filled = (int)((value / maxVal) * len);
    cout << label << ": [";
    for (int i = 0; i < len; i++) {
        if (i < filled) cout << "#";
        else cout << ".";
    }
    cout << "] " << value << " mmHg" << endl;
}

void Display::printStats(float avgInlet, float avgOutlet, float avgError) {
    cout << "\n========================================" << endl;
    cout << "STATISTIK:" << endl;
    cout << "  Medel inlopp:  " << avgInlet << " mmHg" << endl;
    cout << "  Medel utlopp:  " << avgOutlet << " mmHg" << endl;
    cout << "  Medel fel:     " << avgError << " mmHg" << endl;
    cout << "========================================" << endl;
}