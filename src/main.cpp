#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
#include <vector>
#include <cmath>
#include <random>
#include <sys/select.h>
#include <sys/time.h>
#include "../include/constants.h"




#include <algorithm>
#include <numeric>

using namespace std;
static struct termios old_term;




// ==================== PRESSURE SENSOR ====================
class PressureSensor {
private:
    float maxPressure;
    
public:
    PressureSensor(float maxP) : maxPressure(maxP) {}
    
    float measure(float truePressure) {
        static random_device rd;
        static default_random_engine gen(rd());
        static normal_distribution<float> noise(0, Constants::SENSOR_NOISE_LEVEL);
        
        float noisyPressure = truePressure + noise(gen);
        return max(0.0f, min(maxPressure, noisyPressure));
    }
    
    float pressureToVoltage(float pressure) const {
        return Constants::SENSOR_CALIBRATION_ZERO + pressure * Constants::SENSOR_VOLTAGE_PER_MMHG;
    }
};




// ==================== STARLING MECHANISM ====================
// RV: RAP (mmHg) → target PAP (mmHg)
float starlingRight(float rap) {
    using namespace Constants;
    
    if (rap <= 0) return 0;
    if (rap <= STARLING_RV_BREAK1) {
        return rap * STARLING_RV_SLOPE1;
    }
    if (rap <= STARLING_RV_BREAK2) {
        return STARLING_RV_VALUE1 + (rap - STARLING_RV_BREAK1) * STARLING_RV_SLOPE2;
    }
    if (rap <= STARLING_RV_BREAK3) {
        return STARLING_RV_VALUE2 + (rap - STARLING_RV_BREAK2) * STARLING_RV_SLOPE3;
    }
    return STARLING_RV_MAX;
}

// LV: LAP (mmHg) → target AoP (mmHg)
float starlingLeft(float lap) {
    using namespace Constants;
    
    if (lap <= 0) return 0;
    if (lap <= STARLING_LV_BREAK1) {
        return lap * STARLING_LV_SLOPE1;
    }
    if (lap <= STARLING_LV_BREAK2) {
        return STARLING_LV_VALUE1 + (lap - STARLING_LV_BREAK1) * STARLING_LV_SLOPE2;
    }
    if (lap <= STARLING_LV_BREAK3) {
        return STARLING_LV_VALUE2 + (lap - STARLING_LV_BREAK2) * STARLING_LV_SLOPE3;
    }
    return STARLING_LV_MAX;
}

// ==================== PUMP MOTOR ====================
class Motor {
private:
    float setpointRPM = 0.0f;
    float currentRPM = 0.0f;
    float currentVoltage = 0.0f;
    float maxPressure;           // Maximum pressure this motor can generate
    float maxRPM;
    float maxVoltage;
    float tau;          // First-order time constant (seconds)
    
public:
    Motor(float maxP) : maxPressure(maxP), maxRPM(Constants::MOTOR_MAX_RPM), 
                        maxVoltage(Constants::MOTOR_MAX_VOLTAGE),
                        tau(Constants::TAU) {}
    
    void initialize(float defaultRPM) {
        currentRPM = defaultRPM;
        setpointRPM = defaultRPM;
        currentVoltage = rpmToVoltage(currentRPM);
    }
    
    float pressureToRPM(float pressure) const {
        float normalizedPressure = pressure / maxPressure;
        float rpm = sqrt(normalizedPressure) * maxRPM;
        return max(0.0f, min(maxRPM, rpm));
    }
    
    float rpmToPressure(float rpm) const {
        float normalizedRPM = rpm / maxRPM;
        float pressure = maxPressure * normalizedRPM * normalizedRPM;
        return min(maxPressure, pressure);
    }
    
    float rpmToVoltage(float rpm) const {
        return (rpm / maxRPM) * maxVoltage;
    }
    
    void setTargetPressure(float targetPressure) {
        setpointRPM = pressureToRPM(targetPressure);
    }
    
    float getSetpointRPM() const { return setpointRPM; }
    float getSetpointVoltage() const { return rpmToVoltage(setpointRPM); }
    
    // First-order dynamics: approaches setpoint with time constant τ
    // change = (setpoint - current) * (dt / τ)
    void update(float dt) {
        float diff = setpointRPM - currentRPM;
        float gain = dt / tau;
        currentRPM += diff * gain;
        currentRPM = max(0.0f, min(maxRPM, currentRPM));
        currentVoltage = rpmToVoltage(currentRPM);
    }
    
    float getActualPressure() const { return rpmToPressure(currentRPM); }
    float getActualRPM() const { return currentRPM; }
    float getActualVoltage() const { return currentVoltage; }
    
    void reduceSpeed(float reductionFactor) {
        setpointRPM *= reductionFactor;
        currentRPM *= reductionFactor;
    }
    
    float getMaxPressure() const { return maxPressure; }
};

// ==================== CIRCULATION DYNAMICS ====================
// Based on Realheart paper: how the body responds to pump output
float calculateNewRAP(float actualAoP, float heartRate) {
    using namespace Constants;
    
    // Higher aortic pressure reduces venous return to right atrium
    float aopEffect = actualAoP * AOP_EFFECT_ON_RAP;
    float rap = DEFAULT_RAP - aopEffect;
    rap = max(MIN_RAP, min(MAX_RAP, rap));
    
    // Faster heart rate increases venous return
    rap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    
    return rap;
}

float calculateNewLAP(float actualPAP, float heartRate) {
    using namespace Constants;
    
    // Higher pulmonary pressure reduces venous return to left atrium
    float papEffect = actualPAP * PAP_EFFECT_ON_LAP;
    float lap = DEFAULT_LAP - papEffect;
    lap = max(MIN_LAP, min(MAX_LAP, lap));
    
    // Faster heart rate increases venous return
    lap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    
    return lap;
}

// ==================== TERMINAL FUNCTIONS ====================
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
                  float rap, float papTarget, float papActual, float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, float rpmLeft, float vLeft, float errorLeft) {
    
    // Avrunda ALLT till heltal (inga decimaler)
    cout << " " << setw(4) << static_cast<int>(time);  // Trunkera till hel sekund
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
    int attempts = 0;
    while (attempts < 600) {  // 30 sekunder timeout
        if (keyPressed()) {
            char c = getKey();
            if (c == 's' || c == 'q') {
                return c;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(Constants::KEY_POLL_MS));
        attempts++;
    }
    cout << "\nTimeout - avslutar...\n";
    return 'q';
}

bool handleUserInput(float& hr, bool& running) {
    if (keyPressed()) {
        char c = getKey();
        if (c == 'u') {
            hr += Constants::HR_STEP;
            if (hr > Constants::MAX_HR) hr = Constants::MAX_HR;
            printPulseMessage(hr);
            return true;
        } else if (c == 'd') {
            hr -= Constants::HR_STEP;
            if (hr < Constants::MIN_HR) hr = Constants::MIN_HR;
            printPulseMessage(hr);
            return true;
        } else if (c == 'q') {
            running = false;
            return false;
        }
    }
    return false;
}

// ==================== MAIN SIMULATION LOOP ====================
int main() {
    using namespace Constants;
    
    // Sätt upp terminal först
    setupTerminal();
    
    float heartRate = DEFAULT_HR;
    float simTime = 0.0f;
    bool running = true;
    
    PressureSensor rapSensor(MAX_RV_PRESSURE);   // Measures RAP (0-40 mmHg)
    PressureSensor lapSensor(MAX_LV_PRESSURE);   // Measures LAP (0-120 mmHg)
    
    Motor rightPump(MAX_RV_PRESSURE);   // Right ventricle: pumps to lungs (0-40 mmHg)
    Motor leftPump(MAX_LV_PRESSURE);    // Left ventricle: pumps to body (0-120 mmHg)
    
    // Initialize motors with correct default RPM for resting state
    rightPump.initialize(DEFAULT_RPM_RV);
    leftPump.initialize(DEFAULT_RPM_LV);
    
    // State variables
    float rap = DEFAULT_RAP;   // Right Atrial Pressure (mmHg)
    float lap = DEFAULT_LAP;   // Left Atrial Pressure (mmHg)
    
    printHeader();
    
    char startKey = waitForStart();
    if (startKey == 'q') {
        restoreTerminal();
        cout << "Simulering avbruten.\n";
        return 0;
    }
    
    this_thread::sleep_for(chrono::milliseconds(500));


    
    for (int i = 0; i < TOTAL_ITERATIONS && running; i++) {
        handleUserInput(heartRate, running);
        
        // Read sensors (with noise)
        float measuredRAP = rapSensor.measure(rap);
        float measuredLAP = lapSensor.measure(lap);
        
        // ==================== RV ====================
        float targetPAP = starlingRight(measuredRAP);
        rightPump.setTargetPressure(targetPAP);
        rightPump.update(DT);
        float actualPAP = rightPump.getActualPressure();
        float actualVoltageRight = rightPump.getActualVoltage();
        float errorRight = targetPAP - actualPAP;
        
        // ==================== LV ====================
        float targetAoP = starlingLeft(measuredLAP);
        leftPump.setTargetPressure(targetAoP);
        leftPump.update(DT);
        float actualAoP = leftPump.getActualPressure();
        float actualVoltageLeft = leftPump.getActualVoltage();
        float errorLeft = targetAoP - actualAoP;
        
        // ==================== ANTI-SUCTION ====================
        if (measuredRAP < SUCTION_THRESHOLD_RV) {
            rightPump.reduceSpeed(0.8f);
        }
        if (measuredLAP < SUCTION_THRESHOLD_LV) {
            leftPump.reduceSpeed(0.8f);
        }
        
        // ==================== CIRCULATION MODEL ====================
        float newRAP = calculateNewRAP(actualAoP, heartRate);
        float newLAP = calculateNewLAP(actualPAP, heartRate);
        rap = newRAP;
        lap = newLAP;
        
               // ==================== DISPLAY ====================
             
        printDataRow(simTime, heartRate,
                     measuredRAP, targetPAP, actualPAP, rightPump.getSetpointRPM(), actualVoltageRight, errorRight,
                     measuredLAP, targetAoP, actualAoP, leftPump.getSetpointRPM(), actualVoltageLeft, errorLeft);
        
        simTime += DT;
        this_thread::sleep_for(chrono::milliseconds(SLEEP_MS));
    }
    
    printSummary(TOTAL_ITERATIONS);
    restoreTerminal();
    
    return 0;
}