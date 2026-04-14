/**
 * @file Simulation.h
 * @brief Main controller class for TAH simulation
 * @author Malena Grannerud
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "model/PressureSensor.h"
#include "model/Motor.h"
#include "model/StarlingCurve.h"
#include "model/CirculationModel.h"
#include "model/Statistics.h"
#include "view/Terminal.h"

/**
 * @class Simulation
 * @brief Controller coordinating Model and View in MVC architecture
 * 
 * Handles:
 * - User input (heart rate changes, quit)
 * - Model component coordination (sensors, pumps, curves, circulation)
 * - Main simulation loop timing
 * - Statistics collection
 */
class Simulation {
public:
    Simulation();
    void run();
    
private:
    void handleUserInput();
    
    int m_heartRate;
    int m_simTime;
    bool m_running;
    int m_rap;
    int m_lap;
    
    PressureSensor m_rapSensor;
    PressureSensor m_lapSensor;
    Motor m_rightPump;
    Motor m_leftPump;
    StarlingCurve m_starlingRV;
    StarlingCurve m_starlingLV;
    
    Statistics m_stats;
};

#endif