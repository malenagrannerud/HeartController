/**
 * @file Constants.h
 * @brief All constants for TAH simulation
 * 
 * Contains physiological constants, motor parameters, and simulation timing.
 * Based on Realheart paper and device specifications.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {

// ========== PHYSIOLOGICAL CONSTANTS ==========
constexpr float DEFAULT_HR = 72.0f;             // Default heart rate at rest (bpm)
constexpr float MIN_HR = 40.0f;                 // Minimum allowed heart rate (bpm)
constexpr float MAX_HR = 180.0f;                // Maximum allowed heart rate (bpm)
constexpr float HR_STEP = 5.0f;                 // Heart rate change per keypress (bpm)

// ========== RIGHT VENTRICLE ==========
constexpr float MAX_RV_PRESSURE = 40.0f;        // Maximum pressure right ventricle (mmHg)
constexpr float MIN_RV_PRESSURE = 0.0f;          // Minimum pressure (mmHg)
constexpr float RAP_SETPOINT = 4.0f;             // Target RAP at rest (mmHg)
constexpr float SUCTION_THRESHOLD_RV = 2.0f;     // Below this, reduce pump speed (mmHg)

// Starling Mechanism RV - RAP -> target PAP
constexpr float STARLING_RV_BREAK1 = 2.0f;       // First breakpoint (mmHg)
constexpr float STARLING_RV_BREAK2 = 6.0f;       // Second breakpoint (mmHg)
constexpr float STARLING_RV_BREAK3 = 12.0f;      // Third breakpoint (mmHg)
constexpr float STARLING_RV_VALUE1 = 12.0f;      // Target PAP at break1 (mmHg)
constexpr float STARLING_RV_VALUE2 = 22.0f;      // Target PAP at break2 (mmHg)
constexpr float STARLING_RV_MAX = 35.0f;         // Maximum target PAP (mmHg)
constexpr float STARLING_RV_SLOPE1 = 6.0f;       // Slope 0-2 mmHg
constexpr float STARLING_RV_SLOPE2 = 2.5f;       // Slope 2-6 mmHg
constexpr float STARLING_RV_SLOPE3 = 2.17f;      // Slope 6-12 mmHg

// ========== LEFT VENTRICLE ==========
constexpr float MAX_LV_PRESSURE = 120.0f;        // Maximum pressure left ventricle (mmHg)
constexpr float MIN_LV_PRESSURE = 0.0f;          // Minimum pressure (mmHg)
constexpr float LAP_SETPOINT = 9.0f;             // Target LAP at rest (mmHg)
constexpr float SUCTION_THRESHOLD_LV = 2.0f;     // Below this, reduce pump speed (mmHg)

// Starling Mechanism LV - LAP -> target AoP
constexpr float STARLING_LV_BREAK1 = 5.0f;       // First breakpoint (mmHg)
constexpr float STARLING_LV_BREAK2 = 12.0f;      // Second breakpoint (mmHg)
constexpr float STARLING_LV_BREAK3 = 20.0f;      // Third breakpoint (mmHg)
constexpr float STARLING_LV_VALUE1 = 70.0f;      // Target AoP at break1 (mmHg)
constexpr float STARLING_LV_VALUE2 = 100.0f;     // Target AoP at break2 (mmHg)
constexpr float STARLING_LV_MAX = 120.0f;        // Maximum target AoP (mmHg)
constexpr float STARLING_LV_SLOPE1 = 14.0f;      // Slope 0-5 mmHg
constexpr float STARLING_LV_SLOPE2 = 4.29f;      // Slope 5-12 mmHg
constexpr float STARLING_LV_SLOPE3 = 2.5f;       // Slope 12-20 mmHg

// ========== CIRCULATION DYNAMICS ==========
constexpr float DEFAULT_RAP = RAP_SETPOINT;      // Default RAP at rest (mmHg)
constexpr float DEFAULT_LAP = LAP_SETPOINT;      // Default LAP at rest (mmHg)
constexpr float MIN_RAP = 2.0f;                  // Minimum possible RAP (mmHg)
constexpr float MAX_RAP = 12.0f;                 // Maximum possible RAP (mmHg)
constexpr float MIN_LAP = 5.0f;                  // Minimum possible LAP (mmHg)
constexpr float MAX_LAP = 20.0f;                 // Maximum possible LAP (mmHg)
constexpr float AOP_EFFECT_ON_RAP = 0.05f;       // mmHg decrease per mmHg AoP
constexpr float PAP_EFFECT_ON_LAP = 0.08f;       // mmHg decrease per mmHg PAP
constexpr float HR_EFFECT_ON_VENOUS_RETURN = 0.05f; // mmHg increase per bpm above 72

// ========== PUMP MOTOR CONSTANTS ==========
constexpr float MOTOR_MAX_RPM = 4720.0f;          // Maximum pump speed (RPM)
constexpr float MOTOR_MAX_VOLTAGE = 12.0f;        // Maximum motor voltage (V)
constexpr float TAU = 0.25f;                     // Time constant for motor response (s)

// ========== PRESSURE SENSOR CONSTANTS ==========
constexpr float SENSOR_CALIBRATION_ZERO = 0.093f;   // Voltage at 0 mmHg (V)
constexpr float SENSOR_VOLTAGE_PER_MMHG = 0.0097f;  // Voltage increase per mmHg (V/mmHg)
constexpr float SENSOR_NOISE_LEVEL = 0.05f;         // Standard deviation of noise (mmHg)

// ========== DEFAULT RPM CALCULATIONS ==========
constexpr float DEFAULT_RPM_RV = 3337.0f;        // Right pump RPM at rest
constexpr float DEFAULT_RPM_LV = 4021.0f;        // Left pump RPM at rest

// ========== SIMULATION TIMING ==========
constexpr float DT = 0.05f;                      // Time step (s) - 50 ms
constexpr float SIM_DURATION = 20.0f;            // Total simulation time (s)
constexpr int TOTAL_ITERATIONS = static_cast<int>(SIM_DURATION / DT); // Number of iterations
constexpr int SLEEP_MS = 50;                     // Sleep between iterations (ms)
constexpr int KEY_POLL_MS = 100;                 // Key press polling interval (ms)

} // namespace Constants

#endif