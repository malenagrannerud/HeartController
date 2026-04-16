#include "Terminal.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <iomanip>
#include <cstdio>

static struct termios old_term;

void setupTerminal() {
    struct termios new_term;
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
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
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) return c;
    return 0;
}

void printHeader() {
    std::cout << "\033[2J\033[H\n";
    std::cout << "==============================================================================================================\n";
    std::cout << "                                    TAH CONTROL MONITOR\n";
    std::cout << "==============================================================================================================\n\n";
    std::cout << "  COMMANDS\n";
    std::cout << "  [s] start\n";
    std::cout << "  [u] increase heart rate +5\n";
    std::cout << "  [d] decrease heart rate -5\n";
    std::cout << "  [q] quit\n\n";
    
    std::cout << "              |     RIGHT VENTRICLE            |         LEFT VENTRICLE         |                \n";
    std::cout << "Time   HR     |  RAP    CO_RV    PAP     RPM_R |  LAP    CO_LV    AoP    RPM_L  | CO Bal  | ALARM\n";
    std::cout << "[sec] [BPM]   | [mmHg] [L/min]  [mmHg]  [rpm]  | [mmHg] [L/min]  [mmHg] [rpm]   |[RV/LV]  |      \n";
    std::cout << "-------------------------------------------------------------------------------------------------------------------------\n";
    std::cout.flush();
}

void printPulseMessage(float hr) {
    std::cout << "\r    (((" << static_cast<int>(hr) << ")))                    \n";
    std::cout.flush();
}





void printDataRow(float time, float hr, 
                  float rap, float coRV, float pap, float rpmRight,
                  float lap, float coLV, float aop, float rpmLeft, 
                  float balance,
                  const char* alarm) {
    
    // Time och HR
    std::cout << std::setw(5) << std::left << static_cast<int>(time) << "  ";
    std::cout << std::setw(4) << std::left << static_cast<int>(hr) << "   | ";
    
    // RIGHT VENTRICLE: RAP, CO_RV, PAP, RPM_R
    std::cout << std::setw(5) << std::right << static_cast<int>(rap + 0.5f) << "  ";
    std::cout << std::setw(7) << std::fixed << std::setprecision(1) << coRV << "  ";
    std::cout << std::setw(6) << static_cast<int>(pap + 0.5f) << "  ";
    std::cout << std::setw(6) << static_cast<int>(rpmRight + 0.5f) << "  | ";
    
    // LEFT VENTRICLE: LAP, CO_LV, AoP, RPM_L
    std::cout << std::setw(5) << std::right << static_cast<int>(lap + 0.5f) << "  ";
    std::cout << std::setw(7) << std::fixed << std::setprecision(1) << coLV << "  ";
    std::cout << std::setw(6) << static_cast<int>(aop + 0.5f) << "  ";
    std::cout << std::setw(6) << static_cast<int>(rpmLeft + 0.5f) << "   | ";
    
    // CO Bal (Balance)
    if (balance < 0.9f || balance > 1.1f) {
        std::cout << std::setw(7) << " ***   | ";
    } else {
        std::cout << std::setw(7) << std::fixed << std::setprecision(2) << balance << "  | ";
    }
    
    // ALARM
    std::cout << alarm << std::endl;
    std::cout.flush();
}






char waitForStart() {
    std::cout << "\nPress 's' to start, 'q' to quit: ";
    std::cout.flush();
    while (true) {
        char c = std::getchar();
        if (c == 's' || c == 'S') { std::cout << "\nStarting...\n\n"; return 's'; }
        if (c == 'q' || c == 'Q') { std::cout << "\nQuitting...\n"; return 'q'; }
    }
}

void printSummary(int count, float avgHR, float avgCO, float avgBalance,
                  float avgRAP, float avgPAP, float avgLAP, float avgAoP,
                  float minRAP, float maxRAP, float minPAP, float maxPAP,
                  float minLAP, float maxLAP, float minAoP, float maxAoP,
                  float avgErrorRight, float avgErrorLeft) {
    
    std::cout << "\n\n==============================================================================================================\n";
    std::cout << "                                    SIMULATION SUMMARY\n";
    std::cout << "==============================================================================================================\n\n";
    std::cout << "Data points: " << count << " | Duration: " << static_cast<int>(count * 0.05f) << "s\n";
    std::cout << "HR: " << static_cast<int>(avgHR + 0.5f) << " bpm | CO: " << std::fixed << std::setprecision(1) << avgCO << " L/min\n";
    
    std::cout << "RV/LV Balance: " << std::fixed << std::setprecision(2) << avgBalance;
    if (avgBalance < 0.9f || avgBalance > 1.1f) {
        std::cout << " *** UNBALANCED! ***";
    } else {
        std::cout << " (BALANCED)";
    }
    std::cout << "\n\n";
    
    bool rapNormal = (avgRAP >= 2.0f && avgRAP <= 8.0f);
    bool papNormal = (avgPAP >= 15.0f && avgPAP <= 25.0f);
    bool lapNormal = (avgLAP >= 5.0f && avgLAP <= 12.0f);
    bool aopNormal = (avgAoP >= 70.0f && avgAoP <= 100.0f);
    bool balanceNormal = (avgBalance >= 0.9f && avgBalance <= 1.1f);
    
    std::cout << "CLINICAL ASSESSMENT\n";
    std::cout << "--------------------------------------------------------------------------------------------------------------\n";
    std::cout << "  " << (rapNormal ? "[OK]" : "[!!]") << " RAP: " << static_cast<int>(avgRAP + 0.5f) << " (" << static_cast<int>(minRAP + 0.5f) << "-" << static_cast<int>(maxRAP + 0.5f) << ") mmHg\n";
    std::cout << "  " << (papNormal ? "[OK]" : "[!!]") << " PAP: " << static_cast<int>(avgPAP + 0.5f) << " (" << static_cast<int>(minPAP + 0.5f) << "-" << static_cast<int>(maxPAP + 0.5f) << ") mmHg\n";
    std::cout << "  " << (lapNormal ? "[OK]" : "[!!]") << " LAP: " << static_cast<int>(avgLAP + 0.5f) << " (" << static_cast<int>(minLAP + 0.5f) << "-" << static_cast<int>(maxLAP + 0.5f) << ") mmHg\n";
    std::cout << "  " << (aopNormal ? "[OK]" : "[!!]") << " AoP: " << static_cast<int>(avgAoP + 0.5f) << " (" << static_cast<int>(minAoP + 0.5f) << "-" << static_cast<int>(maxAoP + 0.5f) << ") mmHg\n";
    std::cout << "  " << (balanceNormal ? "[OK]" : "[!!]") << " RV/LV Balance: " << std::fixed << std::setprecision(2) << avgBalance << "\n";
    std::cout << "\n==============================================================================================================\n";
    std::cout.flush();
}