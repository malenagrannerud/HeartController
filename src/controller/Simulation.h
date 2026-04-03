#include "Simulation.h"
#include "CirculationModel.h"
#include "Terminal.h"
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>

// Simulation constants
constexpr int DEFAULT_HR = 72;
constexpr int MIN_HR = 40;
constexpr int MAX_HR = 180;
constexpr int HR_STEP = 5;
constexpr int MAX_RV_PRESSURE = 40;
constexpr int MAX_LV_PRESSURE = 120;
constexpr int DEFAULT_RAP = 4;
constexpr int DEFAULT_LAP = 9;
constexpr int SUCTION_THRESHOLD = 2;
constexpr int DT_MS = 50;
constexpr int SIM_DURATION_SEC = 20;
constexpr int TOTAL_ITERATIONS = SIM_DURATION_SEC * 1000 / DT_MS;
constexpr int SLEEP_MS = 50;

Simulation::Simulation()
    : m_heartRate(DEFAULT_HR)
    , m_simTime(0.0f)
    , m_running(true)
    , m_rap(static_cast<float>(DEFAULT_RAP))
    , m_lap(static_cast<float>(DEFAULT_LAP))
    , m_rapSensor(static_cast<float>(MAX_RV_PRESSURE))
    , m_lapSensor(static_cast<float>(MAX_LV_PRESSURE))
    , m_rightPump(static_cast<float>(MAX_RV_PRESSURE))
    , m_leftPump(static_cast<float>(MAX_LV_PRESSURE))
    , m_starlingRV(StarlingCurve::getDefaultRVPoints())
    , m_starlingLV(StarlingCurve::getDefaultLVPoints())
{
    float defaultRPM_RV = Motor::MAX_RPM * std::sqrt(22.0f / static_cast<float>(MAX_RV_PRESSURE));
    float defaultRPM_LV = Motor::MAX_RPM * std::sqrt(87.0f / static_cast<float>(MAX_LV_PRESSURE));
    
    m_rightPump.initialize(defaultRPM_RV);
    m_leftPump.initialize(defaultRPM_LV);
}

Simulation::~Simulation() {}

void Simulation::handleUserInput() {
    if (keyPressed()) {
        char c = getKey();
        if (c == 'u') {
            m_heartRate += HR_STEP;
            if (m_heartRate > MAX_HR) m_heartRate = MAX_HR;
            printPulseMessage(static_cast<float>(m_heartRate));
        } else if (c == 'd') {
            m_heartRate -= HR_STEP;
            if (m_heartRate < MIN_HR) m_heartRate = MIN_HR;
            printPulseMessage(static_cast<float>(m_heartRate));
        } else if (c == 'q') {
            m_running = false;
        }
    }
}

void Simulation::run() {
    setupTerminal();
    printHeader();
    
    char startKey = waitForStart();
    if (startKey == 'q') {
        restoreTerminal();
        std::cout << "Simulering avbruten.\n";
        return;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    float dt = static_cast<float>(DT_MS) / 1000.0f;
    float simDuration = static_cast<float>(SIM_DURATION_SEC);
    float currentTime = 0.0f;
    
    for (int i = 0; i < TOTAL_ITERATIONS && m_running; i++) {
        handleUserInput();
        
        float measuredRAP = m_rapSensor.measure(m_rap);
        float measuredLAP = m_lapSensor.measure(m_lap);
        
        float targetPAP = m_starlingRV.evaluate(measuredRAP);
        m_rightPump.setTargetPressure(targetPAP);
        m_rightPump.update(dt);
        float actualPAP = m_rightPump.getActualPressure();
        float actualVoltageRight = m_rightPump.getActualVoltage();
        float errorRight = targetPAP - actualPAP;
        
        float targetAoP = m_starlingLV.evaluate(measuredLAP);
        m_leftPump.setTargetPressure(targetAoP);
        m_leftPump.update(dt);
        float actualAoP = m_leftPump.getActualPressure();
        float actualVoltageLeft = m_leftPump.getActualVoltage();
        float errorLeft = targetAoP - actualAoP;
        
        if (measuredRAP < static_cast<float>(SUCTION_THRESHOLD)) m_rightPump.reduceSpeed(0.8f);
        if (measuredLAP < static_cast<float>(SUCTION_THRESHOLD)) m_leftPump.reduceSpeed(0.8f);
        
        m_rap = calculateNewRAP(actualAoP, static_cast<float>(m_heartRate));
        m_lap = calculateNewLAP(actualPAP, static_cast<float>(m_heartRate));
        
        m_stats.add(static_cast<float>(m_heartRate), measuredRAP, measuredLAP, actualPAP, actualAoP, errorRight, errorLeft);
        
        printDataRow(currentTime, static_cast<float>(m_heartRate),
                     measuredRAP, targetPAP, actualPAP, 
                     m_rightPump.getSetpointRPM(), actualVoltageRight, errorRight,
                     measuredLAP, targetAoP, actualAoP, 
                     m_leftPump.getSetpointRPM(), actualVoltageLeft, errorLeft);
        
        currentTime += dt;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
    }
    
    printSummary(TOTAL_ITERATIONS,
                 m_stats.avg(m_stats.hr),
                 m_stats.avg(m_stats.rap),
                 m_stats.avg(m_stats.lap),
                 m_stats.avg(m_stats.pap),
                 m_stats.avg(m_stats.aop),
                 m_stats.avg(m_stats.errorRight),
                 m_stats.avg(m_stats.errorLeft),
                 m_stats.minVal(m_stats.rap),
                 m_stats.maxVal(m_stats.rap),
                 m_stats.minVal(m_stats.lap),
                 m_stats.maxVal(m_stats.lap),
                 m_stats.minVal(m_stats.pap),
                 m_stats.maxVal(m_stats.pap),
                 m_stats.minVal(m_stats.aop),
                 m_stats.maxVal(m_stats.aop));
    
    restoreTerminal();
}