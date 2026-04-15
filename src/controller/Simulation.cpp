#include "Simulation.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>

Simulation::Simulation()
    : m_heartRate(DEFAULT_HR)
    , m_simTime(0)
    , m_running(true)
    , m_circulation()
    , m_rapSensor(MAX_RV_PRESSURE)
    , m_lapSensor(MAX_LV_PRESSURE)
    , m_rightPump(MAX_RV_PRESSURE)
    , m_leftPump(MAX_LV_PRESSURE)
    , m_starlingRV(StarlingCurve::getDefaultRVPoints())
    , m_starlingLV(StarlingCurve::getDefaultLVPoints())
    , m_stats() {
    
    float defaultRPM_RV = Motor::MAX_RPM * std::sqrt(22.0f / MAX_RV_PRESSURE);
    float defaultRPM_LV = Motor::MAX_RPM * std::sqrt(87.0f / MAX_LV_PRESSURE);
    m_rightPump.initialize(defaultRPM_RV);
    m_leftPump.initialize(defaultRPM_LV);
    m_circulation.reset();
}

void Simulation::handleUserInput() {
    if (keyPressed()) {
        char c = getKey();
        if (c == 'u') {
            m_heartRate += HR_STEP;
            if (m_heartRate > MAX_HR) m_heartRate = MAX_HR;
            printPulseMessage(m_heartRate);
        } else if (c == 'd') {
            m_heartRate -= HR_STEP;
            if (m_heartRate < MIN_HR) m_heartRate = MIN_HR;
            printPulseMessage(m_heartRate);
        } else if (c == 'q') {
            m_running = false;
        }
    }
}

void Simulation::run() {
    setupTerminal();
    printHeader();
    
    if (waitForStart() == 'q') {
        restoreTerminal();
        return;
    }
    
    float dt = static_cast<float>(DT_MS) / 1000.0f;
    
    for (int i = 0; i < TOTAL_ITERATIONS && m_running; ++i) {
        handleUserInput();
        
        float trueRAP = m_circulation.getRAP();
        float trueLAP = m_circulation.getLAP();
        float measuredRAP = m_rapSensor.measure(trueRAP);
        float measuredLAP = m_lapSensor.measure(trueLAP);
        
        float targetPAP = m_starlingRV.evaluate(measuredRAP);
        float targetAoP = m_starlingLV.evaluate(measuredLAP);
        
        m_rightPump.setTargetPressure(targetPAP);
        m_rightPump.update(dt);
        float actualPAP = m_rightPump.getActualPressure();
        float errorRight = targetPAP - actualPAP;
        
        m_leftPump.setTargetPressure(targetAoP);
        m_leftPump.update(dt);
        float actualAoP = m_leftPump.getActualPressure();
        float errorLeft = targetAoP - actualAoP;
        
        if (measuredRAP < SUCTION_THRESHOLD) m_rightPump.reduceSpeed(0.8f);
        if (measuredLAP < SUCTION_THRESHOLD) m_leftPump.reduceSpeed(0.8f);
        
        m_circulation.update(static_cast<float>(m_heartRate), actualPAP, actualAoP);
        
        m_stats.record(m_heartRate,
                      static_cast<int>(measuredRAP), static_cast<int>(measuredLAP),
                      static_cast<int>(actualPAP), static_cast<int>(actualAoP),
                      static_cast<int>(errorRight), static_cast<int>(errorLeft));
        
        const char* alarm = "OK";
        if (m_circulation.getRAP() < SUCTION_THRESHOLD) alarm = "*** SUCTION RV! ***";
        else if (m_circulation.getLAP() < SUCTION_THRESHOLD) alarm = "*** SUCTION LV! ***";
        
        // Ersätt printDataRow-anropet med:
printDataRow(
    static_cast<float>(m_simTime),
    static_cast<float>(m_heartRate),
    m_circulation.getCO_RV(),
    m_circulation.getCO_LV(),
    m_circulation.getBalance(),
    m_circulation.getRAP(), m_circulation.getPAP(),
    m_rightPump.getSetpointRPM(), m_rightPump.getActualVoltage(), errorRight,
    m_circulation.getLAP(), m_circulation.getAoP(),
    m_leftPump.getSetpointRPM(), m_leftPump.getActualVoltage(), errorLeft,
    alarm
);
        
        m_simTime += static_cast<int>(dt);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
    }
    
    printSummary(
    m_stats.getCount(),
    static_cast<float>(m_stats.avgHR()), 
    m_circulation.getCO(),
    m_circulation.getBalance(),
    static_cast<float>(m_stats.avgRAP()), static_cast<float>(m_stats.avgPAP()),
    static_cast<float>(m_stats.avgLAP()), static_cast<float>(m_stats.avgAoP()),
    static_cast<float>(m_stats.minRAP()), static_cast<float>(m_stats.maxRAP()),
    static_cast<float>(m_stats.minPAP()), static_cast<float>(m_stats.maxPAP()),
    static_cast<float>(m_stats.minLAP()), static_cast<float>(m_stats.maxLAP()),
    static_cast<float>(m_stats.minAoP()), static_cast<float>(m_stats.maxAoP()),
    static_cast<float>(m_stats.avgErrorRight()), static_cast<float>(m_stats.avgErrorLeft())
);
    
    restoreTerminal();
}