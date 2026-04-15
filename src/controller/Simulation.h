/**
 * @file Simulation.h
 * @brief Huvudkontroller för TAH-simulering
 * 
 * KOPPLINGAR:
 * - Sensorer mäter RAP/LAP från CirculationModel
 * - StarlingCurve omvandlar RAP/LAP → target PAP/AoP
 * - Motorerna (pumparna) försöker nå target-trycken
 * - CirculationModel får faktiska tryck och räknar ut nya RAP/LAP
 * - Display visar allt
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
    static constexpr int DEFAULT_HR = 72;
    static constexpr int MIN_HR = 40;
    static constexpr int MAX_HR = 180;
    static constexpr int HR_STEP = 5;
    static constexpr int MAX_RV_PRESSURE = 40;
    static constexpr int MAX_LV_PRESSURE = 120;
    static constexpr int SUCTION_THRESHOLD = 2;
    static constexpr int DT_MS = 50;
    static constexpr int SIM_DURATION_SEC = 20;
    static constexpr int TOTAL_ITERATIONS = SIM_DURATION_SEC * 1000 / DT_MS;
    static constexpr int SLEEP_MS = 50;
    
    void handleUserInput();
    
    int m_heartRate;
    int m_simTime;
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