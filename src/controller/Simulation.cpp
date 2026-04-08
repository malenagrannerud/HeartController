#include "Simulation.h"
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <numeric>
#include <algorithm>

static constexpr int DEFAULT_HR = 72;
static constexpr int MIN_HR = 40;
static constexpr int MAX_HR = 180;
static constexpr int HR_STEP = 5;
static constexpr int MAX_RV_PRESSURE = 40;
static constexpr int MAX_LV_PRESSURE = 120;
static constexpr int DEFAULT_RAP = 4;
static constexpr int DEFAULT_LAP = 9;
static constexpr int SUCTION_THRESHOLD = 2;
static constexpr int DT_MS = 50;
static constexpr int SIM_DURATION_SEC = 20;
static constexpr int TOTAL_ITERATIONS = SIM_DURATION_SEC * 1000 / DT_MS;
static constexpr int SLEEP_MS = 50;

void Simulation::Stats::add(int h, int r, int la, int p, int a, int er, int el) {
    hr.push_back(h); rap.push_back(r); lap.push_back(la);
    pap.push_back(p); aop.push_back(a); errR.push_back(er); errL.push_back(el);
}

int Simulation::Stats::avg(const std::vector<int>& v) {
    return v.empty() ? 0 : std::accumulate(v.begin(), v.end(), 0) / v.size();
}

int Simulation::Stats::min(const std::vector<int>& v) {
    return v.empty() ? 0 : *std::min_element(v.begin(), v.end());
}

int Simulation::Stats::max(const std::vector<int>& v) {
    return v.empty() ? 0 : *std::max_element(v.begin(), v.end());
}

Simulation::Simulation()
    : m_heartRate(DEFAULT_HR), m_simTime(0), m_running(true),
      m_rap(DEFAULT_RAP), m_lap(DEFAULT_LAP),
      m_rapSensor(MAX_RV_PRESSURE), m_lapSensor(MAX_LV_PRESSURE),
      m_rightPump(MAX_RV_PRESSURE), m_leftPump(MAX_LV_PRESSURE),
      m_starlingRV(StarlingCurve::getDefaultRVPoints()),
      m_starlingLV(StarlingCurve::getDefaultLVPoints()) {
    
    float defaultRPM_RV = Motor::MAX_RPM * std::sqrt(22.0f / MAX_RV_PRESSURE);
    float defaultRPM_LV = Motor::MAX_RPM * std::sqrt(87.0f / MAX_LV_PRESSURE);
    m_rightPump.initialize(defaultRPM_RV);
    m_leftPump.initialize(defaultRPM_LV);
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
        std::cout << "Simulation aborted.\n";
        return;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int dt = DT_MS / 1000;
    
    for (int i = 0; i < TOTAL_ITERATIONS && m_running; i++) {
        handleUserInput();
        
        int measuredRAP = m_rapSensor.measure(m_rap);
        int measuredLAP = m_lapSensor.measure(m_lap);
        
        int targetPAP = m_starlingRV.evaluate(measuredRAP);
        m_rightPump.setTargetPressure(targetPAP);
        m_rightPump.update(dt);
        int actualPAP = m_rightPump.getActualPressure();
        int errorRight = targetPAP - actualPAP;
        
        int targetAoP = m_starlingLV.evaluate(measuredLAP);
        m_leftPump.setTargetPressure(targetAoP);
        m_leftPump.update(dt);
        int actualAoP = m_leftPump.getActualPressure();
        int errorLeft = targetAoP - actualAoP;
        
        if (measuredRAP < SUCTION_THRESHOLD) m_rightPump.reduceSpeed(0.8);
        if (measuredLAP < SUCTION_THRESHOLD) m_leftPump.reduceSpeed(0.8);
        
        m_rap = calculateNewRAP(actualAoP, m_heartRate);
        m_lap = calculateNewLAP(actualPAP, m_heartRate);
        
        m_stats.add(m_heartRate, measuredRAP, measuredLAP, actualPAP, actualAoP, errorRight, errorLeft);
        
        printDataRow(m_simTime, m_heartRate,
                     measuredRAP, targetPAP, actualPAP,
                     m_rightPump.getSetpointRPM(), m_rightPump.getActualVoltage(), errorRight,
                     measuredLAP, targetAoP, actualAoP,
                     m_leftPump.getSetpointRPM(), m_leftPump.getActualVoltage(), errorLeft);
        
        m_simTime += dt;
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
    }
    
    printSummary(TOTAL_ITERATIONS,
                 m_stats.avg(m_stats.hr), m_stats.avg(m_stats.rap), m_stats.avg(m_stats.lap),
                 m_stats.avg(m_stats.pap), m_stats.avg(m_stats.aop),
                 m_stats.avg(m_stats.errR), m_stats.avg(m_stats.errL),
                 m_stats.min(m_stats.rap), m_stats.max(m_stats.rap),
                 m_stats.min(m_stats.lap), m_stats.max(m_stats.lap),
                 m_stats.min(m_stats.pap), m_stats.max(m_stats.pap),
                 m_stats.min(m_stats.aop), m_stats.max(m_stats.aop));
    
    restoreTerminal();
}



