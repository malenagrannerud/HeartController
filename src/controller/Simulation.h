/**
 * @file Simulation.h
 * @brief Main controller class for TAH simulation
 * 
 * This class implements the main control loop for a TAH simulation. 
 * It coordinates model components and handles user interaction.
 * 
 * @author Malena Grannerud
 * @date 2026-04-09
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
 * This class:
 * - Runs the main simulation loop (20 seconds, 50ms steps)
 * - Handles user input (u/d to change heart rate, q to quit)
 * - Coordinates sensors, pumps, Starling curves, and circulation model
 * - Collects statistics and displays data in real-time
 * 
 * The control loop executes every 50 milliseconds:
 * 0. Calculate trueRAP and trueLAP using the circulation model
 * 1. Read pressure sensors (RAP, LAP), by taking trueRAP and trueLAP and adding noise
 * 2. Calculate target pressures using Starling curves
 * 3. Update motor speeds (first-order dynamics)
 * 4. Update circulation model based on actual pressures
 * 5. Display data and collect statistics
 */

class Simulation {
public:
    /**
     * @brief Constructor
     * Initializes all components with default values:
     * - Default heart rate: 72 bpm --> trueRAP: 4 mmHg, trueLAP: 9 mmHg, DEFAULT_RPM
     * - Starling curves with default breakpoints
     */
    Simulation();
    
    /**
     * @brief Run the main simulation
     * 
     * This function:
     * - Sets up terminal and displays header
     * - Waits for user to press 's' to start
     * - Runs the main loop for SIM_DURATION_SEC seconds
     * - Displays summary statistics after completion
     * - Restores terminal on exit
     */
    void run();
    
private:
    /**
     * @brief Handle user keyboard input during simulation
     * 
     * Polls for keypresses and responds to:
     * - 'u': Increase heart rate by HR_STEP (max MAX_HR)
     * - 'd': Decrease heart rate by HR_STEP (min MIN_HR)
     * - 'q': Stop simulation
     * 
     * Displays a pulse animation when heart rate changes.
     */
    void handleUserInput();
    
    // ========== Simulation State ==========
    int m_heartRate;        ///< Current heart rate in beats per minute (40-180)
    int m_simTime;          ///< Current simulation time in milliseconds
    bool m_running;         ///< Flag to control simulation loop
    
    // ========== Physiological State ==========
    int m_rap;              ///< Right Atrial Pressure (mmHg) - preload for RV
    int m_lap;              ///< Left Atrial Pressure (mmHg) - preload for LV
    
    // ========== Model Components ==========
    PressureSensor m_rapSensor;     ///< Sensor measuring RAP
    PressureSensor m_lapSensor;     ///< Sensor measuring LAP
    Motor m_rightPump;              ///< Right ventricle pump (to lungs, max 40 mmHg)
    Motor m_leftPump;               ///< Left ventricle pump (to body, max 120 mmHg)
    StarlingCurve m_starlingRV;     ///< Frank-Starling curve for right ventricle (RAP → PAP)
    StarlingCurve m_starlingLV;     ///< Frank-Starling curve for left ventricle (LAP → AoP)
    
    // ========== Statistics ==========
    Statistics m_stats;             ///< Collects and stores simulation data
};

#endif // SIMULATION_H