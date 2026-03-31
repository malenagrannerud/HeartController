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
    cout << "====================================================================================================\n";
    cout << "                                    TAH SIMULATION\n";
    cout << "====================================================================================================\n\n";
    cout << "  COMMANDS\n";
    cout << "  [s] start\n";
    cout << "  [u] increase HR\n";
    cout << "  [d] decrease HR\n";
    cout << "  [q] quit\n\n";

    cout << "Time  HR    RV:RAP  PAPt  PAPa  RPM  V  Error       LV:LAP  AoPt AoPa RPM  V Error    \n";
    cout << "--------------------------------------------------------------------------------------\n";
}

void printPulseMessage(float hr) {
    cout << "\t(((" << static_cast<int>(hr) << ")))\n";
}

void printDataRow(float time, float hr,
                  float rap, float papTarget, float papActual, 
                  float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, 
                  float rpmLeft, float vLeft, float errorLeft) {
    
    cout << " " << setw(4) << static_cast<int>(time);
    cout << "  " << setw(3) << static_cast<int>(hr);
    cout << "  " << setw(4) << static_cast<int>(rap + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(papTarget + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(papActual + 0.5f);
    cout << "  " << setw(5) << static_cast<int>(rpmRight);
    cout << "  " << setw(4) << static_cast<int>(vRight + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(errorRight + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(lap + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(aopTarget + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(aopActual + 0.5f);
    cout << "  " << setw(5) << static_cast<int>(rpmLeft);
    cout << "  " << setw(3) << static_cast<int>(vLeft + 0.5f);
    cout << "  " << setw(4) << static_cast<int>(errorLeft + 0.5f) << endl;
}

void printSummary(int count) {
    cout << "\n\n====================================================================================================\n";
    cout << "  SIMULATION COMPLETE - " << count << " data points\n";
    cout << "====================================================================================================\n";
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
    cout << "\nTimeout - avslutar...\n";
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