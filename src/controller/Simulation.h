/**
 * @file Simulation.h
 * @brief Main controller for TAH simulation
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "../model/PressureSensor.h"
#include "../model/Motor.h"
#include "../model/StarlingCurve.h"
#include "../model/CirculationModel.h"
#include "../model/Statistics.h"
#include "../view/Terminal.h"

class Simulation {
public:
    Simulation();
    void run();
    
private:
    static constexpr int DEFAULT_HR = 72; // Normal HR at rest
    static constexpr int MIN_HR = 40;
    static constexpr int MAX_HR = 200;
    static constexpr int HR_STEP = 5;
    static constexpr int SUCTION_THRESHOLD = 2;
    static constexpr int DT_MS = 50;
    static constexpr int SIM_DURATION_SEC = 20;
    static constexpr int TOTAL_ITERATIONS = SIM_DURATION_SEC * 1000 / DT_MS;
    static constexpr int SLEEP_MS = 50;
    
    void handleUserInput();
    
    int m_heartRate;
    float m_simTime;
    bool m_running;
    
    CirculationModel m_circulation;
    PressureSensor m_rapSensor;
    PressureSensor m_lapSensor;
    Motor m_rightPump;
    Motor m_leftPump;
    StarlingCurve m_starlingRV;
    StarlingCurve m_starlingLV;
    Statistics m_stats;
};

#endif