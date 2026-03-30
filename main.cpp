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

using namespace std;
struct termios old_term;

// ============================================================================
// SIMPLIFIED TOTAL ARTIFICIAL HEART CONTROLLER
// ============================================================================
// ABBREVIATIONS:
//   HR  = Heart Rate (bpm) - user can adjust to simulate exercise or rest
//   RAP = Right Atrial Pressure (mmHg) - blood pressure entering right ventricle from body
//   PAP = Pulmonary Artery Pressure (mmHg) - blood pressure exiting right ventricle to lungs
//   LAP = Left Atrial Pressure (mmHg) - blood pressure entering left ventricle from lungs
//   AoP = Aortic Pressure (mmHg) - blood pressure exiting left ventricle to body
//
//   MCU = MICROCONTROLLER
//   RPM = Revolutions Per Minute (pump speed)
// ============================================================================
//
//                 ----------------------BODY----------------------------
//                 ↑                                                     ↓
//                 │ AoP (aorta)                                       ||||| Sensor1: RAP
//   ┌─────────────┴─────────────-┐                        ┌─────────────┴─────────────┐
//   │ LEFT VENTRICLE             │                        │ RIGHT VENTRICLE           │
//   │ Pumps blood from lungs→body│                        │ Pumps body → lungs        │
//   │ Motor2: follows setpoint   │                        │ Motor1: follows setpoint  │
//   │ Driver2: controls motor    │                        │ Driver1: controls motor   │
//   └─────────────┬─────────────-┘                        └─────────────┬─────────────┘
//               ||||| Sensor2: LAP                                      ↓ PAP 
//                 ↑                                                     ↓
//                 └───────────────────────LUNGS─────────────────────────┘
//                                    
//      MCU: 
//      Loop
//          Reads sensors
//          Sets setpoints to motors
//               
// 
// 
// AIM: To model a system for a TAH to understand how 
//      i: HR (input by user), PAP (affects LAP) and AoP (affects RAP) interact  
//      ii: how the Frank-Starling mechanism determines target pressures based on preload
//      iii: how a display can show real-time data to users
//
// The system is programmed to respond to changes in RAP and LAP by adjusting RPM
// to maintain physiological pressures, just like a natural heart follows the Frank-Starling mechanism.
// The user can adjust HR to simulate exercise or rest, and the system will show how the pumps respond to maintain circulation.
// ============================================================================
// ONE MCU CONTROL LOOP
// ============================================================================
//
// Each time step (DT = 50 ms):
//
//   1. Read user input (u/d/q) via USB serial
//      Update HR if key pressed
//
//   2. Read sensors:
//      RAP = analogRead(A0) → convert to mmHg
//      LAP = analogRead(A1) → convert to mmHg
//
//   3. Right ventricle control:
//      targetPAP = starling(RAP)
//      setpointRPM1 = pressureToRPM(targetPAP)
//      setpointVoltage1 = rpmToVoltage(setpointRPM1)
//      updateMotor1(setpointRPM1, DT)
//      actualPAP = motor1.getPressure()
//      errorRight = targetPAP - actualPAP
//
//   4. Left ventricle control:
//      targetAoP = starling(LAP)
//      setpointRPM2 = pressureToRPM(targetAoP)
//      setpointVoltage2 = rpmToVoltage(setpointRPM2)
//      updateMotor2(setpointRPM2, DT)
//      actualAoP = motor2.getPressure()
//      errorLeft = targetAoP - actualAoP
//
//   5. Circulation model:
//      newRAP = calculatePreload(actualAoP, HR)  // AoP affects RAP
//      newLAP = calculatePreload(actualPAP, HR)  // PAP affects LAP
//
//   6. Anti-suction check:
//      if RAP < 2 mmHg → reduce motor1 speed
//      if LAP < 2 mmHg → reduce motor2 speed
//
//   7. Print data row to serial monitor
//
// ============================================================================
//  8. Software includes an anti-suction algorithm:
// If inlet pressure drops suddenly or if pump power consumption starts fluctuating
// (sign of turbulence), the system immediately lowers speed to let the atrium refill.
//

// ==================== SYSTEM CONSTANTS ====================
namespace Constants {
    // ========== PHYSIOLOGICAL CONSTANTS ==========
    // HR - beats per minute
    const float DEFAULT_HR = 72.0f;           // Normal resting heart rate
    const float MIN_HR = 40.0f;               // Minimum allowed HR
    const float MAX_HR = 180.0f;              // Maximum allowed HR (max exertion)
    const float HR_STEP = 5.0f;               // Step change when user presses u/d
    
    // ========== RIGHT VENTRICLE (Pumps body → lungs) ==========
    // Low pressure system - pumps to lungs
    const float MAX_RV_PRESSURE = 40.0f;      // Maximum pressure right ventricle can generate (mmHg)
    const float MIN_RV_PRESSURE = 0.0f;       // Minimum pressure (mmHg)
    const float RAP_SETPOINT = 4.0f;          // mmHg - target right atrial pressure at rest (from Realheart paper)
    const float SUCTION_THRESHOLD_RV = 2.0f;  // mmHg - below this, reduce pump speed
    
    // Starling Mechanism RV (based on Realheart)
    // Maps RAP (preload) → target PAP (afterload)
    // From paper: RAP setpoint 4 mmHg, PAP increases with RAP
    const float STARLING_RV_BREAK1 = 2.0f;     // mmHg - first breakpoint
    const float STARLING_RV_BREAK2 = 6.0f;     // mmHg - second breakpoint
    const float STARLING_RV_BREAK3 = 12.0f;    // mmHg - third breakpoint
    const float STARLING_RV_VALUE1 = 12.0f;    // mmHg - target PAP at 2 mmHg RAP
    const float STARLING_RV_VALUE2 = 22.0f;    // mmHg - target PAP at 6 mmHg RAP
    const float STARLING_RV_MAX = 35.0f;       // mmHg - maximum target PAP
    const float STARLING_RV_SLOPE1 = 6.0f;     // 0-2 mmHg: target increase per mmHg RAP
    const float STARLING_RV_SLOPE2 = 2.5f;     // 2-6 mmHg: target increase per mmHg RAP
    const float STARLING_RV_SLOPE3 = 2.17f;    // 6-12 mmHg: target increase per mmHg RAP
    
    // ========== LEFT VENTRICLE (Pumps lungs → body) ==========
    // High pressure system - pumps to body
    const float MAX_LV_PRESSURE = 120.0f;      // Maximum pressure left ventricle can generate (mmHg)
    const float MIN_LV_PRESSURE = 0.0f;        // Minimum pressure (mmHg)
    const float LAP_SETPOINT = 9.0f;           // mmHg - target left atrial pressure at rest (from Realheart paper)
    const float SUCTION_THRESHOLD_LV = 2.0f;   // mmHg - below this, reduce pump speed
    
    // Frank-Starling Mechanism for Left Ventricle (based on Realheart paper)
    // Maps LAP (preload) → target AoP (afterload)
    // From paper: LAP setpoint 9 mmHg, AoP increases with LAP
    const float STARLING_LV_BREAK1 = 5.0f;     // mmHg - first breakpoint
    const float STARLING_LV_BREAK2 = 12.0f;    // mmHg - second breakpoint
    const float STARLING_LV_BREAK3 = 20.0f;    // mmHg - third breakpoint
    const float STARLING_LV_VALUE1 = 70.0f;    // mmHg - target AoP at 5 mmHg LAP
    const float STARLING_LV_VALUE2 = 100.0f;   // mmHg - target AoP at 12 mmHg LAP
    const float STARLING_LV_MAX = 120.0f;      // mmHg - maximum target AoP
    const float STARLING_LV_SLOPE1 = 14.0f;    // 0-5 mmHg: target increase per mmHg LAP
    const float STARLING_LV_SLOPE2 = 4.29f;    // 5-12 mmHg: target increase per mmHg LAP
    const float STARLING_LV_SLOPE3 = 2.5f;     // 12-20 mmHg: target increase per mmHg LAP
    
    // ========== CIRCULATION DYNAMICS ==========
    // How RAP and LAP change based on pump output and HR
    const float DEFAULT_RAP = RAP_SETPOINT;     // mmHg - RAP at rest
    const float DEFAULT_LAP = LAP_SETPOINT;     // mmHg - LAP at rest
    const float MIN_RAP = 2.0f;                // mmHg - minimum possible RAP
    const float MAX_RAP = 12.0f;               // mmHg - maximum possible RAP
    const float MIN_LAP = 5.0f;                // mmHg - minimum possible LAP
    const float MAX_LAP = 20.0f;               // mmHg - maximum possible LAP
    
    // Effect of AoP on RAP (venous return to right side)
    // From Realheart paper: AoP affects venous return
    const float AOP_EFFECT_ON_RAP = 0.05f;      // mmHg - RAP decreases by 0.05 per mmHg AoP
    
    // Effect of PAP on LAP (venous return to left side)
    // From Realheart paper: PAP affects venous return to left atrium
    const float PAP_EFFECT_ON_LAP = 0.08f;      // mmHg - LAP decreases by 0.08 per mmHg PAP
    
    // Effect of HR on both RAP and LAP
    // From Realheart paper: HR increases during exercise, affecting venous return
    const float HR_EFFECT_ON_VENOUS_RETURN = 0.05f;  // mmHg - increase per bpm above 72
    
    // ========== PUMP MOTOR CONSTANTS ==========
    const float MOTOR_MAX_RPM = 4720.0f;        // Maximum pump speed
    const float MOTOR_MAX_VOLTAGE = 12.0f;      // Maximum motor voltage (volts)
    const float TAU = 0.25f;    // Seconds - time constant for motor response (τ)
    // First-order dynamics: motor speed follows setpoint with time constant τ
    // update: currentRPM += (setpointRPM - currentRPM) * (dt / τ)
    
    // ========== PRESSURE SENSOR CONSTANTS ==========
    const float SENSOR_CALIBRATION_ZERO = 0.093f;    // Voltage at 0 mmHg
    const float SENSOR_VOLTAGE_PER_MMHG = 0.0097f;   // Voltage increase per mmHg
    const float SENSOR_NOISE_LEVEL = 0.05f;          // Standard deviation of noise (mmHg)
    
    // ========== DEFAULT RPM CALCULATIONS ==========
    // RV at rest: RAP = 4 mmHg
    // targetPAP = starlingRight(4) = 15 + (4-2)*2.5 = 20 mmHg
    // DEFAULT_RPM_RV = sqrt(20 / MAX_RV_PRESSURE) * MOTOR_MAX_RPM
    //                    = sqrt(0.5) * 4720 = 0.707 * 4720 = 3337 RPM
    const float DEFAULT_RPM_RV = 3337.0f;
    
    // LV at rest: LAP = 9 mmHg
    // targetAoP = starlingLeft(9) = 70 + (9-5)*4.29 = 70 + 17.16 = 87.16 mmHg
    // DEFAULT_RPM_LV = sqrt(87.16 / MAX_LV_PRESSURE) * MOTOR_MAX_RPM
    //                  = sqrt(0.726) * 4720 = 0.852 * 4720 = 4021 RPM
    const float DEFAULT_RPM_LV = 4021.0f;
    
    // ========== SIMULATION TIMING ==========
    const float DT = 0.05f;                    // Time step (seconds) - 50 ms
    const float SIM_DURATION = 10.0f;          // Total simulation time (seconds)
    const int TOTAL_ITERATIONS = static_cast<int>(SIM_DURATION / DT);
    const int SLEEP_MS = 50;                   // Sleep between iterations
    const int KEY_POLL_MS = 100;               // How often to check for key press
}

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

// ==================== STARLING MECHANISMS ====================
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
    tcgetattr(0, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new_term);
}

void restoreTerminal() {
    tcsetattr(0, TCSANOW, &old_term);
}

bool keyPressed() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

char getKey() {
    char buf = 0;
    read(0, &buf, 1);
    return buf;
}

void printHeader() {
    cout << "\n";
    cout << "========================================================================================================\n";
    cout << "                                    TAH SIMULATION\n";
    cout << "========================================================================================================\n\n";
    cout << "  COMMANDS\n";
    cout << "  [s] start\n";
    cout << "  [u] increase HR\n";
    cout << "  [d] decrease HR\n";
    cout << "  [q] quit\n\n";

    cout << "  Time|HR   |RAP   |PAP   |PAP_actual|RPM_R|V_RV|Err_RV|LAP   |AoP   |AoP_actual|RPM_LV|V_LV|Err_LV \n";
    cout << "  [s] |[bpm]|[mmHg]|[mmHg]|[mmHg]    |[rpm]|[V] |[mmHg]|[mmHg]|[mmHg]|[mmHg]    |[rpm] |[V] |[mmHg] \n";
    cout << "  ------------------------------------------------------------------------------------------------------\n";
}

void printPulseMessage(float hr) {
    cout << "\t(((" << hr << ")))\n";
}

void printDataRow(float time, float hr,
                  float rap, float papTarget, float papActual, float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, float rpmLeft, float vLeft, float errorLeft) {
    cout << fixed << setprecision(2);
    cout << "   " << setw(5) << time;
    cout << "   " << setw(5) << hr;
    cout << "   " << setw(6) << rap;
    cout << "   " << setw(8) << papTarget;
    cout << "   " << setw(8) << papActual;
    cout << "   " << setw(7) << static_cast<int>(rpmRight);
    cout << "   " << setw(5) << vRight;
    cout << "   " << setw(7) << errorRight;
    cout << "   " << setw(6) << lap;
    cout << "   " << setw(8) << aopTarget;
    cout << "   " << setw(8) << aopActual;
    cout << "   " << setw(7) << static_cast<int>(rpmLeft);
    cout << "   " << setw(5) << vLeft;
    cout << "   " << setw(7) << errorLeft << endl;
}

void printSummary(int count) {
    cout << "\n\n========================================================================================================\n";
    cout << "  SIMULATION COMPLETE - " << count << " data points\n";
    cout << "========================================================================================================\n";
}

char waitForStart() {
    while (true) {
        if (keyPressed()) {
            char c = getKey();
            if (c == 's' || c == 'q') {
                return c;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(Constants::KEY_POLL_MS));
    }
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
    
    setupTerminal();
    printHeader();
    
    char startKey = waitForStart();
    if (startKey == 'q') {
        restoreTerminal();
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