#include "Terminal.h"
#include "Constants.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <iomanip>

using namespace std;

static struct termios old_term;

void setupTerminal() {
    struct termios new_term;
    if (tcgetattr(STDIN_FILENO, &old_term) != 0) {
        cerr << "Fel vid inställning av terminal\n";
        return;
    }
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    new_term.c_cc[VMIN] = 0;
    new_term.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0) {
        cerr << "Fel vid applicering av terminalinställningar\n";
    }
}

void restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}

bool keyPressed() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char getKey() {
    char buf = 0;
    if (read(STDIN_FILENO, &buf, 1) > 0) {
        return buf;
    }
    return 0;
}

void printHeader() {
    cout << "\n";
    cout << "===============================================================================\n";
    cout << "                         TAH SIMULATION\n";
    cout << "===============================================================================\n\n";
    cout << "  COMMANDS\n";
    cout << "  [s] start\n";
    cout << "  [u] increase heart rate\n";
    cout << "  [d] decrease heart rate\n";
    cout << "  [q] quit\n\n";

    cout << "Time  HR           RAP PAPt PAPa  RPM  V  Err        LAP AoPt AoPa  RPM  V  Err\n";
    cout << "--------------------------------------------------------------------------------------\n";
}

void printPulseMessage(float hr) {
    cout << "    (((" << static_cast<int>(hr) << ")))\n";
}

void printDataRow(float time, float hr,
                  float rap, float papTarget, float papActual, 
                  float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, 
                  float rpmLeft, float vLeft, float errorLeft) {
    
    cout << setw(4) << static_cast<int>(time);
    cout << "  ";
    cout << setw(3) << static_cast<int>(hr);
    cout << "        ";
    cout << setw(3) << static_cast<int>(rap + 0.5f);
    cout << " ";
    cout << setw(4) << static_cast<int>(papTarget + 0.5f);
    cout << " ";
    cout << setw(4) << static_cast<int>(papActual + 0.5f);
    cout << "  ";
    cout << setw(4) << static_cast<int>(rpmRight);
    cout << "  ";
    cout << setw(1) << static_cast<int>(vRight + 0.5f);
    cout << " ";
    cout << setw(3) << static_cast<int>(errorRight + 0.5f);
    cout << "        ";
    cout << setw(3) << static_cast<int>(lap + 0.5f);
    cout << " ";
    cout << setw(4) << static_cast<int>(aopTarget + 0.5f);
    cout << " ";
    cout << setw(4) << static_cast<int>(aopActual + 0.5f);
    cout << "  ";
    cout << setw(4) << static_cast<int>(rpmLeft);
    cout << "  ";
    cout << setw(1) << static_cast<int>(vLeft + 0.5f);
    cout << " ";
    cout << setw(3) << static_cast<int>(errorLeft + 0.5f) << endl;
}

void printSummary(int count, float avgHR, float avgRAP, float avgLAP,
                  float avgPAP, float avgAoP, float avgErrorRight, float avgErrorLeft,
                  float minRAP, float maxRAP, float minLAP, float maxLAP,
                  float minPAP, float maxPAP, float minAoP, float maxAoP) {
    
    cout << "\n\n";
    cout << "===============================================================================\n";
    cout << "                         SIMULATION SUMMARY\n";
    cout << "===============================================================================\n\n";
    
    cout << "GENERAL INFORMATION\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << "  Data points collected: " << count << "\n";
    cout << "  Simulation duration:   " << static_cast<int>(count * 0.05f) << " seconds\n\n";
    
    cout << "HEART RATE\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << "  Average HR:  " << static_cast<int>(avgHR + 0.5f) << " bpm\n\n";
    
    cout << "RIGHT VENTRICLE\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << "  RAP (Right Atrial Pressure):\n";
    cout << "    Min:  " << static_cast<int>(minRAP + 0.5f) << " mmHg\n";
    cout << "    Max:  " << static_cast<int>(maxRAP + 0.5f) << " mmHg\n";
    cout << "    Avg:  " << static_cast<int>(avgRAP + 0.5f) << " mmHg\n\n";
    
    cout << "  PAP (Pulmonary Artery Pressure):\n";
    cout << "    Min:  " << static_cast<int>(minPAP + 0.5f) << " mmHg\n";
    cout << "    Max:  " << static_cast<int>(maxPAP + 0.5f) << " mmHg\n";
    cout << "    Avg:  " << static_cast<int>(avgPAP + 0.5f) << " mmHg\n\n";
    
    cout << "  Control Error (Target - Actual):\n";
    cout << "    Avg: " << static_cast<int>(avgErrorRight + 0.5f) << " mmHg\n\n";
    
    cout << "LEFT VENTRICLE\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << "  LAP (Left Atrial Pressure):\n";
    cout << "    Min:  " << static_cast<int>(minLAP + 0.5f) << " mmHg\n";
    cout << "    Max:  " << static_cast<int>(maxLAP + 0.5f) << " mmHg\n";
    cout << "    Avg:  " << static_cast<int>(avgLAP + 0.5f) << " mmHg\n\n";
    
    cout << "  AoP (Aortic Pressure):\n";
    cout << "    Min:  " << static_cast<int>(minAoP + 0.5f) << " mmHg\n";
    cout << "    Max:  " << static_cast<int>(maxAoP + 0.5f) << " mmHg\n";
    cout << "    Avg:  " << static_cast<int>(avgAoP + 0.5f) << " mmHg\n\n";
    
    cout << "  Control Error (Target - Actual):\n";
    cout << "    Avg: " << static_cast<int>(avgErrorLeft + 0.5f) << " mmHg\n\n";
    
    cout << "CLINICAL ASSESSMENT\n";
    cout << "-------------------------------------------------------------------------------\n";
    
    bool rapNormal = (avgRAP >= 2.0f && avgRAP <= 6.0f);
    bool lapNormal = (avgLAP >= 5.0f && avgLAP <= 12.0f);
    bool papNormal = (avgPAP >= 15.0f && avgPAP <= 25.0f);
    bool aopNormal = (avgAoP >= 70.0f && avgAoP <= 100.0f);
    bool hrNormal = (avgHR >= 60.0f && avgHR <= 100.0f);
    
    cout << "  " << (hrNormal ? "[OK]" : "[!!]") << " Heart Rate: " 
         << static_cast<int>(avgHR + 0.5f) << " bpm\n";
    cout << "  " << (rapNormal ? "[OK]" : "[!!]") << " Right Atrial Pressure: " 
         << static_cast<int>(avgRAP + 0.5f) << " mmHg\n";
    cout << "  " << (lapNormal ? "[OK]" : "[!!]") << " Left Atrial Pressure: " 
         << static_cast<int>(avgLAP + 0.5f) << " mmHg\n";
    cout << "  " << (papNormal ? "[OK]" : "[!!]") << " Pulmonary Artery Pressure: " 
         << static_cast<int>(avgPAP + 0.5f) << " mmHg\n";
    cout << "  " << (aopNormal ? "[OK]" : "[!!]") << " Aortic Pressure: " 
         << static_cast<int>(avgAoP + 0.5f) << " mmHg\n";
    
    cout << "\n";
    cout << "-------------------------------------------------------------------------------\n";
    
    int normalCount = (rapNormal ? 1 : 0) + (lapNormal ? 1 : 0) + 
                      (papNormal ? 1 : 0) + (aopNormal ? 1 : 0) + (hrNormal ? 1 : 0);
    
    cout << "OVERALL STATUS: ";
    if (normalCount == 5) {
        cout << "NORMAL\n";
    } else if (normalCount >= 3) {
        cout << "CAUTION - " << (5 - normalCount) << " parameter(s) outside normal range\n";
    } else {
        cout << "CRITICAL - Check pump settings\n";
    }
    
    cout << "\n===============================================================================\n";
    cout << "  SIMULATION COMPLETE - " << count << " data points\n";
    cout << "===============================================================================\n";
}

char waitForStart() {
    using namespace Constants;
    
    int attempts = 0;
    while (attempts < 600) {
        if (keyPressed()) {
            char c = getKey();
            if (c == 's' || c == 'q') {
                return c;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(KEY_POLL_MS));
        attempts++;
    }
    cout << "\nTimeout - shutting down...\n";
    return 'q';
}

bool handleUserInput(float& hr, bool& running) {
    using namespace Constants;
    
    if (keyPressed()) {
        char c = getKey();
        if (c == 'u') {
            hr += HR_STEP;
            if (hr > MAX_HR) hr = MAX_HR;
            printPulseMessage(hr);
            return true;
        } else if (c == 'd') {
            hr -= HR_STEP;
            if (hr < MIN_HR) hr = MIN_HR;
            printPulseMessage(hr);
            return true;
        } else if (c == 'q') {
            running = false;
            return false;
        }
    }
    return false;
}