// Main simulation loop for TAH controller
// Real-time heart simulation with Frank-Starling control

#include "Constants.h"
#include "PressureSensor.h"
#include "StarlingMechanism.h"
#include "Motor.h"
#include "CirculationModel.h"
#include "Terminal.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace Constants;

struct Statistics {
    vector<float> hr;
    vector<float> rap;
    vector<float> lap;
    vector<float> pap;
    vector<float> aop;
    vector<float> errorRight;
    vector<float> errorLeft;
    
    void add(float _hr, float _rap, float _lap, float _pap, float _aop, 
             float _errR, float _errL) {
        hr.push_back(_hr);
        rap.push_back(_rap);
        lap.push_back(_lap);
        pap.push_back(_pap);
        aop.push_back(_aop);
        errorRight.push_back(_errR);
        errorLeft.push_back(_errL);
    }
    
    float avg(const vector<float>& v) {
        if (v.empty()) return 0;
        return accumulate(v.begin(), v.end(), 0.0f) / v.size();
    }
    
    float minVal(const vector<float>& v) {
        if (v.empty()) return 0;
        return *min_element(v.begin(), v.end());
    }
    
    float maxVal(const vector<float>& v) {
        if (v.empty()) return 0;
        return *max_element(v.begin(), v.end());
    }
};

int main() {
    setupTerminal();
    
    float heartRate = DEFAULT_HR;
    float simTime = 0.0f;
    bool running = true;
    
    Statistics stats;
    
    PressureSensor rapSensor(MAX_RV_PRESSURE);
    PressureSensor lapSensor(MAX_LV_PRESSURE);
    
    Motor rightPump(MAX_RV_PRESSURE);
    Motor leftPump(MAX_LV_PRESSURE);
    
    rightPump.initialize(DEFAULT_RPM_RV);
    leftPump.initialize(DEFAULT_RPM_LV);
    
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
        
        // Read sensors with noise
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
        
        // Collect statistics
        stats.add(heartRate, measuredRAP, measuredLAP, actualPAP, actualAoP, errorRight, errorLeft);
        
        // Display
        printDataRow(simTime, heartRate,
                     measuredRAP, targetPAP, actualPAP, 
                     rightPump.getSetpointRPM(), actualVoltageRight, errorRight,
                     measuredLAP, targetAoP, actualAoP, 
                     leftPump.getSetpointRPM(), actualVoltageLeft, errorLeft);
        
        simTime += DT;
        this_thread::sleep_for(chrono::milliseconds(SLEEP_MS));
    }
    
    // Print summary with statistics
    printSummary(TOTAL_ITERATIONS,
                 stats.avg(stats.hr),
                 stats.avg(stats.rap),
                 stats.avg(stats.lap),
                 stats.avg(stats.pap),
                 stats.avg(stats.aop),
                 stats.avg(stats.errorRight),
                 stats.avg(stats.errorLeft),
                 stats.minVal(stats.rap),
                 stats.maxVal(stats.rap),
                 stats.minVal(stats.lap),
                 stats.maxVal(stats.lap),
                 stats.minVal(stats.pap),
                 stats.maxVal(stats.pap),
                 stats.minVal(stats.aop),
                 stats.maxVal(stats.aop));
    
    restoreTerminal();
    
    return 0;
}