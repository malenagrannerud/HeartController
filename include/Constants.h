#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {

// ========== PHYSIOLOGICAL CONSTANTS ==========
constexpr float DEFAULT_HR = 72.0f;
constexpr float MIN_HR = 40.0f;
constexpr float MAX_HR = 180.0f;
constexpr float HR_STEP = 5.0f;

// ========== RIGHT VENTRICLE ==========
constexpr float MAX_RV_PRESSURE = 40.0f;
constexpr float MIN_RV_PRESSURE = 0.0f;
constexpr float RAP_SETPOINT = 4.0f;
constexpr float SUCTION_THRESHOLD_RV = 2.0f;

// Starling Mechanism RV
constexpr float STARLING_RV_BREAK1 = 2.0f;
constexpr float STARLING_RV_BREAK2 = 6.0f;
constexpr float STARLING_RV_BREAK3 = 12.0f;
constexpr float STARLING_RV_VALUE1 = 12.0f;
constexpr float STARLING_RV_VALUE2 = 22.0f;
constexpr float STARLING_RV_MAX = 35.0f;
constexpr float STARLING_RV_SLOPE1 = 6.0f;
constexpr float STARLING_RV_SLOPE2 = 2.5f;
constexpr float STARLING_RV_SLOPE3 = 2.17f;

// ========== LEFT VENTRICLE ==========
constexpr float MAX_LV_PRESSURE = 120.0f;
constexpr float MIN_LV_PRESSURE = 0.0f;
constexpr float LAP_SETPOINT = 9.0f;
constexpr float SUCTION_THRESHOLD_LV = 2.0f;

// Starling Mechanism LV
constexpr float STARLING_LV_BREAK1 = 5.0f;
constexpr float STARLING_LV_BREAK2 = 12.0f;
constexpr float STARLING_LV_BREAK3 = 20.0f;
constexpr float STARLING_LV_VALUE1 = 70.0f;
constexpr float STARLING_LV_VALUE2 = 100.0f;
constexpr float STARLING_LV_MAX = 120.0f;
constexpr float STARLING_LV_SLOPE1 = 14.0f;
constexpr float STARLING_LV_SLOPE2 = 4.29f;
constexpr float STARLING_LV_SLOPE3 = 2.5f;

// ========== CIRCULATION DYNAMICS ==========
constexpr float DEFAULT_RAP = RAP_SETPOINT;
constexpr float DEFAULT_LAP = LAP_SETPOINT;
constexpr float MIN_RAP = 2.0f;
constexpr float MAX_RAP = 12.0f;
constexpr float MIN_LAP = 5.0f;
constexpr float MAX_LAP = 20.0f;
constexpr float AOP_EFFECT_ON_RAP = 0.05f;
constexpr float PAP_EFFECT_ON_LAP = 0.08f;
constexpr float HR_EFFECT_ON_VENOUS_RETURN = 0.05f;

// ========== PUMP MOTOR CONSTANTS ==========
constexpr float MOTOR_MAX_RPM = 4720.0f;
constexpr float MOTOR_MAX_VOLTAGE = 12.0f;
constexpr float TAU = 0.25f;

// ========== PRESSURE SENSOR CONSTANTS ==========
constexpr float SENSOR_CALIBRATION_ZERO = 0.093f;
constexpr float SENSOR_VOLTAGE_PER_MMHG = 0.0097f;
constexpr float SENSOR_NOISE_LEVEL = 0.05f;

// ========== DEFAULT RPM CALCULATIONS ==========
constexpr float DEFAULT_RPM_RV = 3337.0f;
constexpr float DEFAULT_RPM_LV = 4021.0f;

// ========== SIMULATION TIMING ==========
constexpr float DT = 0.05f;
constexpr float SIM_DURATION = 20.0f;
constexpr int TOTAL_ITERATIONS = static_cast<int>(SIM_DURATION / DT);
constexpr int SLEEP_MS = 50;
constexpr int KEY_POLL_MS = 100;

} 

#endif // CONSTANTS_H

