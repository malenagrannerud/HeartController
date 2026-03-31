/**
 * @file Terminal.h
 * @brief Terminal input/output for TAH simulation
 */

#ifndef TERMINAL_H
#define TERMINAL_H

/**
 * @brief Setup terminal for direct key input (no enter, no echo)
 */
void setupTerminal();

/**
 * @brief Restore terminal to normal mode
 */
void restoreTerminal();

/**
 * @brief Check if a key is pressed
 * @return true if key available
 */
bool keyPressed();

/**
 * @brief Read pressed key
 * @return char The key pressed
 */
char getKey();

/**
 * @brief Show menu and column headers
 */
void printHeader();

/**
 * @brief Show one row of simulation data
 * @param time Simulation time (seconds)
 * @param hr Heart rate (bpm)
 * @param rap Right Atrial Pressure (mmHg)
 * @param papTarget Target PAP (mmHg)
 * @param papActual Actual PAP (mmHg)
 * @param rpmRight Right pump speed (RPM)
 * @param vRight Right motor voltage (V)
 * @param errorRight Control error (mmHg)
 * @param lap Left Atrial Pressure (mmHg)
 * @param aopTarget Target AoP (mmHg)
 * @param aopActual Actual AoP (mmHg)
 * @param rpmLeft Left pump speed (RPM)
 * @param vLeft Left motor voltage (V)
 * @param errorLeft Control error (mmHg)
 */
void printDataRow(float time, float hr,
                  float rap, float papTarget, float papActual, 
                  float rpmRight, float vRight, float errorRight,
                  float lap, float aopTarget, float aopActual, 
                  float rpmLeft, float vLeft, float errorLeft);

/**
 * @brief Show statistics and clinical assessment
 * @param count Number of data points
 * @param avgHR Average heart rate
 * @param avgRAP Average RAP
 * @param avgLAP Average LAP
 * @param avgPAP Average PAP
 * @param avgAoP Average AoP
 * @param avgErrorRight Average error (right)
 * @param avgErrorLeft Average error (left)
 * @param minRAP Minimum RAP
 * @param maxRAP Maximum RAP
 * @param minLAP Minimum LAP
 * @param maxLAP Maximum LAP
 * @param minPAP Minimum PAP
 * @param maxPAP Maximum PAP
 * @param minAoP Minimum AoP
 * @param maxAoP Maximum AoP
 */
void printSummary(int count, float avgHR, float avgRAP, float avgLAP,
                  float avgPAP, float avgAoP, float avgErrorRight, float avgErrorLeft,
                  float minRAP, float maxRAP, float minLAP, float maxLAP,
                  float minPAP, float maxPAP, float minAoP, float maxAoP);

/**
 * @brief Wait for 's' (start) or 'q' (quit)
 * @return 's' or 'q'
 */
char waitForStart();

/**
 * @brief Handle user input during simulation
 * @param hr Heart rate (updated on u/d)
 * @param running Simulation flag (false on q)
 * @return true if heart rate changed
 */
bool handleUserInput(float& hr, bool& running);

/**
 * @brief Show heart rate change animation: ((hr))
 * @param hr Current heart rate
 */
void printPulseMessage(float hr);

#endif