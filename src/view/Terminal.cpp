
#include "Terminal.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <iomanip>

static struct termios old_term;

void setupTerminal() {
    struct termios new_term;
    if (tcgetattr(STDIN_FILENO, &old_term) != 0) {
        std::cerr << "Fel vid inställning av terminal\n";
        return;
    }
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    new_term.c_cc[VMIN] = 0;
    new_term.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0) {
        std::cerr << "Fel vid applicering av terminalinställningar\n";
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
    std::cout << "\n";
    std::cout << "===============================================================================\n";
    std::cout << "                         TAH SIMULATION\n";
    std::cout << "===============================================================================\n\n";
    std::cout << "  COMMANDS\n";
    std::cout << "  [s] start\n";
    std::cout << "  [u] increase heart rate\n";
    std::cout << "  [d] decrease heart rate\n";
    std::cout << "  [q] quit\n\n";
    std::cout << "Time  HR           RAP PAPt PAPa  RPM  V  Err        LAP AoPt AoPa  RPM  V  Err\n";
    std::cout << "--------------------------------------------------------------------------------------\n";
}

void printPulseMessage(float hr) {
    std::cout << "    (((" << static_cast<int>(hr) << ")))\n";
}

void printDataRow(float time, float hr,
                  float rap, float papTarget, float papActual, 
                  float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, 
                  float rpmLeft, float vLeft, float errorLeft) {
    
    std::cout << std::setw(4) << static_cast<int>(time);
    std::cout << "  ";
    std::cout << std::setw(3) << static_cast<int>(hr);
    std::cout << "        ";
    std::cout << std::setw(3) << static_cast<int>(rap + 0.5f);
    std::cout << " ";
    std::cout << std::setw(4) << static_cast<int>(papTarget + 0.5f);
    std::cout << " ";
    std::cout << std::setw(4) << static_cast<int>(papActual + 0.5f);
    std::cout << "  ";
    std::cout << std::setw(4) << static_cast<int>(rpmRight);
    std::cout << "  ";
    std::cout << std::setw(1) << static_cast<int>(vRight + 0.5f);
    std::cout << " ";
    std::cout << std::setw(3) << static_cast<int>(errorRight + 0.5f);
    std::cout << "        ";
    std::cout << std::setw(3) << static_cast<int>(lap + 0.5f);
    std::cout << " ";
    std::cout << std::setw(4) << static_cast<int>(aopTarget + 0.5f);
    std::cout << " ";
    std::cout << std::setw(4) << static_cast<int>(aopActual + 0.5f);
    std::cout << "  ";
    std::cout << std::setw(4) << static_cast<int>(rpmLeft);
    std::cout << "  ";
    std::cout << std::setw(1) << static_cast<int>(vLeft + 0.5f);
    std::cout << " ";
    std::cout << std::setw(3) << static_cast<int>(errorLeft + 0.5f) << std::endl;
}

void printSummary(int count, float avgHR, float avgRAP, float avgLAP,
                  float avgPAP, float avgAoP, float avgErrorRight, float avgErrorLeft,
                  float minRAP, float maxRAP, float minLAP, float maxLAP,
                  float minPAP, float maxPAP, float minAoP, float maxAoP) {
    
    std::cout << "\n\n";
    std::cout << "===============================================================================\n";
    std::cout << "                         SIMULATION SUMMARY\n";
    std::cout << "===============================================================================\n\n";
    
    std::cout << "GENERAL INFORMATION\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "  Data points collected: " << count << "\n";
    std::cout << "  Simulation duration:   " << static_cast<int>(count * 0.05f) << " seconds\n\n";
    
    std::cout << "HEART RATE\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "  Average HR:  " << static_cast<int>(avgHR + 0.5f) << " bpm\n\n";
    
    std::cout << "RIGHT VENTRICLE\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "  RAP (Right Atrial Pressure):\n";
    std::cout << "    Min:  " << static_cast<int>(minRAP + 0.5f) << " mmHg\n";
    std::cout << "    Max:  " << static_cast<int>(maxRAP + 0.5f) << " mmHg\n";
    std::cout << "    Avg:  " << static_cast<int>(avgRAP + 0.5f) << " mmHg\n\n";
    
    std::cout << "  PAP (Pulmonary Artery Pressure):\n";
    std::cout << "    Min:  " << static_cast<int>(minPAP + 0.5f) << " mmHg\n";
    std::cout << "    Max:  " << static_cast<int>(maxPAP + 0.5f) << " mmHg\n";
    std::cout << "    Avg:  " << static_cast<int>(avgPAP + 0.5f) << " mmHg\n\n";
    
    std::cout << "  Control Error (Target - Actual):\n";
    std::cout << "    Avg: " << static_cast<int>(avgErrorRight + 0.5f) << " mmHg\n\n";
    
    std::cout << "LEFT VENTRICLE\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << "  LAP (Left Atrial Pressure):\n";
    std::cout << "    Min:  " << static_cast<int>(minLAP + 0.5f) << " mmHg\n";
    std::cout << "    Max:  " << static_cast<int>(maxLAP + 0.5f) << " mmHg\n";
    std::cout << "    Avg:  " << static_cast<int>(avgLAP + 0.5f) << " mmHg\n\n";
    
    std::cout << "  AoP (Aortic Pressure):\n";
    std::cout << "    Min:  " << static_cast<int>(minAoP + 0.5f) << " mmHg\n";
    std::cout << "    Max:  " << static_cast<int>(maxAoP + 0.5f) << " mmHg\n";
    std::cout << "    Avg:  " << static_cast<int>(avgAoP + 0.5f) << " mmHg\n\n";
    
    std::cout << "  Control Error (Target - Actual):\n";
    std::cout << "    Avg: " << static_cast<int>(avgErrorLeft + 0.5f) << " mmHg\n\n";
    
    std::cout << "CLINICAL ASSESSMENT\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    
    bool rapNormal = (avgRAP >= 2.0f && avgRAP <= 6.0f);
    bool lapNormal = (avgLAP >= 5.0f && avgLAP <= 12.0f);
    bool papNormal = (avgPAP >= 15.0f && avgPAP <= 25.0f);
    bool aopNormal = (avgAoP >= 70.0f && avgAoP <= 100.0f);
    bool hrNormal = (avgHR >= 60.0f && avgHR <= 100.0f);
    
    std::cout << "  " << (hrNormal ? "[OK]" : "[!!]") << " Heart Rate: " 
              << static_cast<int>(avgHR + 0.5f) << " bpm\n";
    std::cout << "  " << (rapNormal ? "[OK]" : "[!!]") << " Right Atrial Pressure: " 
              << static_cast<int>(avgRAP + 0.5f) << " mmHg\n";
    std::cout << "  " << (lapNormal ? "[OK]" : "[!!]") << " Left Atrial Pressure: " 
              << static_cast<int>(avgLAP + 0.5f) << " mmHg\n";
    std::cout << "  " << (papNormal ? "[OK]" : "[!!]") << " Pulmonary Artery Pressure: " 
              << static_cast<int>(avgPAP + 0.5f) << " mmHg\n";
    std::cout << "  " << (aopNormal ? "[OK]" : "[!!]") << " Aortic Pressure: " 
              << static_cast<int>(avgAoP + 0.5f) << " mmHg\n";
    
    std::cout << "\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    
    int normalCount = (rapNormal ? 1 : 0) + (lapNormal ? 1 : 0) + 
                      (papNormal ? 1 : 0) + (aopNormal ? 1 : 0) + (hrNormal ? 1 : 0);
    
    std::cout << "OVERALL STATUS: ";
    if (normalCount == 5) {
        std::cout << "NORMAL\n";
    } else if (normalCount >= 3) {
        std::cout << "CAUTION - " << (5 - normalCount) << " parameter(s) outside normal range\n";
    } else {
        std::cout << "CRITICAL - Check pump settings\n";
    }
    
    std::cout << "\n===============================================================================\n";
    std::cout << "  SIMULATION COMPLETE - " << count << " data points\n";
    std::cout << "===============================================================================\n";
}

char waitForStart() {
    int attempts = 0;
    while (attempts < 600) {
        if (keyPressed()) {
            char c = getKey();
            if (c == 's' || c == 'q') {
                return c;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        attempts++;
    }
    std::cout << "\nTimeout - shutting down...\n";
    return 'q';
}