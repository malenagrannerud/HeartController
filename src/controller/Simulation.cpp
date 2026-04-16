
#include "Simulation.h"
#include <thread>
#include <chrono>
#include <iostream>

Simulation::Simulation()
    : m_heartRate(DEFAULT_HR)
    , m_simTime(0.0f)
    , m_running(true)
    , m_circulation()
    , m_rapSensor()
    , m_lapSensor()
    , m_rightPump(10.0f)
    , m_leftPump(12.0f)
    , m_starlingRV(StarlingCurve::getRVPoints())
    , m_starlingLV(StarlingCurve::getLVPoints())
    , m_stats() {
    
    m_circulation.reset();
    
    float startCO_RV = m_starlingRV.evaluate(m_circulation.getRAP(), DEFAULT_HR);
    float startCO_LV = m_starlingLV.evaluate(m_circulation.getLAP(), DEFAULT_HR);
    
    m_rightPump.setTargetFlow(startCO_RV);
    m_leftPump.setTargetFlow(startCO_LV);
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
        
        float coRV = m_starlingRV.evaluate(measuredRAP, static_cast<float>(m_heartRate));
        float coLV = m_starlingLV.evaluate(measuredLAP, static_cast<float>(m_heartRate));
        
        m_rightPump.setTargetFlow(coRV);
        m_rightPump.update(dt);
        float actualFlowRV = m_rightPump.getActualFlow();
        
        m_leftPump.setTargetFlow(coLV);
        m_leftPump.update(dt);
        float actualFlowLV = m_leftPump.getActualFlow();
        
        if (measuredRAP < SUCTION_THRESHOLD) m_rightPump.reduceSpeed(0.8f);
        if (measuredLAP < SUCTION_THRESHOLD) m_leftPump.reduceSpeed(0.8f);
        
        m_circulation.update(static_cast<float>(m_heartRate), actualFlowRV, actualFlowLV, dt);
        
        float errorRight = coRV - actualFlowRV;
        float errorLeft = coLV - actualFlowLV;
        
        m_stats.record(m_heartRate,
                      static_cast<int>(measuredRAP), static_cast<int>(measuredLAP),
                      static_cast<int>(m_circulation.getPAP()), static_cast<int>(m_circulation.getAoP()),
                      static_cast<int>(errorRight), static_cast<int>(errorLeft));
        
        const char* alarm = "OK";
        if (m_circulation.getRAP() < SUCTION_THRESHOLD) alarm = "*** SUCTION RV! ***";
        else if (m_circulation.getLAP() < SUCTION_THRESHOLD) alarm = "*** SUCTION LV! ***";
        
        printDataRow(
            m_simTime,
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
        
        m_simTime += dt;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
    }
    
    printSummary(
        m_stats.getCount(),
        static_cast<float>(m_stats.avgHR()), 
        m_circulation.getCO_RV(),
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