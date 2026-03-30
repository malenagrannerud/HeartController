
#include "../include/display.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

static bool header_printed = false;

void Display::update(float preload_mmHg, float target_mmHg, float actual_mmHg,
                     float motor_rpm, float motor_current_mA,
                     float heart_rate_bpm, float simulation_time_s) {
    
    if (!header_printed) {
        cout << "================================================================================" << endl;
        cout << "                    TAH Right Ventricle - Frank-Starling" << endl;
        cout << "================================================================================" << endl;
        cout << endl;
        cout << "  Tid(s)   Puls(bpm)   Inlet(mmHg)   Target(mmHg)   Actual(mmHg)   Setpoint(V)   Error(mmHg)   Error(V)" << endl;
        cout << "  -----------------------------------------------------------------------------------------------------" << endl;
        header_printed = true;
    }
    
    float setpoint_volt = target_mmHg * 0.0097;
    float error_mmHg = target_mmHg - actual_mmHg;
    float error_volt = error_mmHg * 0.0097;
    
    cout << "  " << fixed << setprecision(1) << setw(6) << simulation_time_s;
    cout << "   " << setw(6) << heart_rate_bpm;
    cout << "   " << setw(9) << preload_mmHg;
    cout << "   " << setw(10) << target_mmHg;
    cout << "   " << setw(10) << actual_mmHg;
    cout << "   " << setw(10) << setpoint_volt;
    cout << "   " << setw(9) << error_mmHg;
    cout << "   " << setw(7) << error_volt << endl;
}

void Display::showControls() {
    cout << "\n-----------------------------------------------------------------------------------------------------" << endl;
    cout << "  KOMMANDON:  [q] Avsluta   [↑] Öka puls   [↓] Minska puls" << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;
}

void Display::printStats(float avg_preload_mmHg, float avg_afterload_mmHg, float avg_error_mmHg) {
    cout << "\n================================================================================" << endl;
    cout << "  SIMULERING KLAR" << endl;
    cout << "================================================================================" << endl;
    cout << "  Medel inlopp:   " << fixed << setprecision(1) << avg_preload_mmHg << " mmHg" << endl;
    cout << "  Medel utlopp:   " << avg_afterload_mmHg << " mmHg" << endl;
    cout << "  Medel fel:      " << avg_error_mmHg << " mmHg" << endl;
    cout << "================================================================================" << endl;
}
