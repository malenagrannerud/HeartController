#include "Constants.h"
#include "PressureSensor.h"
#include "StarlingMechanism.h"
#include "Motor.h"
#include "CirculationModel.h"
#include "Terminal.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace Constants;

int main() {
    setupTerminal();
    
    float heartRate = DEFAULT_HR;
    float simTime = 0.0f;
    bool running = true;
    
    // Sensors
    PressureSensor rapSensor(MAX_RV_PRESSURE);
    PressureSensor lapSensor(MAX_LV_PRESSURE);
    
    // Pumps
    Motor rightPump(MAX_RV_PRESSURE);
    Motor leftPump(MAX_LV_PRESSURE);
    
    rightPump.initialize(DEFAULT_RPM_RV);
    leftPump.initialize(DEFAULT_RPM_LV);
    
    // State variables
    float rap = DEFAULT_RAP;
    float lap = DEFAULT_LAP;
    
    printHeader();
    
    char startKey = waitForStart();
    if (startKey == 'q') {
        restoreTerminal();
        cout << "Simulering avbruten.\n";
        return 0;
    }
    
    this_thread::sleep_for(chrono::milliseconds(500));
    
    // Main simulation loop
    for (int i = 0; i < TOTAL_ITERATIONS && running; i++) {
        handleUserInput(heartRate, running);
        
        // Read sensors (with noise)
        float measuredRAP = rapSensor.measure(rap);
        float measuredLAP = lapSensor.measure(lap);
        
        // Right ventricle control
        float targetPAP = starlingRight(measuredRAP);
        rightPump.setTargetPressure(targetPAP);
        rightPump.update(DT);
        float actualPAP = rightPump.getActualPressure();
        float actualVoltageRight = rightPump.getActualVoltage();
        float errorRight = targetPAP - actualPAP;
        
        // Left ventricle control
        float targetAoP = starlingLeft(measuredLAP);
        leftPump.setTargetPressure(targetAoP);
        leftPump.update(DT);
        float actualAoP = leftPump.getActualPressure();
        float actualVoltageLeft = leftPump.getActualVoltage();
        float errorLeft = targetAoP - actualAoP;
        
        // Anti-suction protection
        if (measuredRAP < SUCTION_THRESHOLD_RV) {
            rightPump.reduceSpeed(0.8f);
        }
        if (measuredLAP < SUCTION_THRESHOLD_LV) {
            leftPump.reduceSpeed(0.8f);
        }
        
        // Update circulation model
        rap = calculateNewRAP(actualAoP, heartRate);
        lap = calculateNewLAP(actualPAP, heartRate);
        
        // Display
        printDataRow(simTime, heartRate,
                     measuredRAP, targetPAP, actualPAP, 
                     rightPump.getSetpointRPM(), actualVoltageRight, errorRight,
                     measuredLAP, targetAoP, actualAoP, 
                     leftPump.getSetpointRPM(), actualVoltageLeft, errorLeft);
        
        simTime += DT;
        this_thread::sleep_for(chrono::milliseconds(SLEEP_MS));
    }
    
    printSummary(TOTAL_ITERATIONS);
    restoreTerminal();
    
    return 0;
}