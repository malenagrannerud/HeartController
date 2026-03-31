#pragma once

namespace Constants {

    // ========== PHYSIOLOGICAL CONSTANTS ==========
  
    const float DEFAULT_HR = 72.0f;           // Normal resting heart rate
    const float MIN_HR = 40.0f;               // Minimum allowed HR
    const float MAX_HR = 180.0f;              // Maximum allowed HR (max exertion)
    const float HR_STEP = 5.0f;               // Step change when user presses u/d
    
    // ========== RIGHT VENTRICLE (Pumps body → lungs) ==========
    const float MAX_RV_PRESSURE = 40.0f;      // Maximum pressure right ventricle can generate (mmHg)
    const float MIN_RV_PRESSURE = 0.0f;       // Minimum pressure (mmHg)
    const float RAP_SETPOINT = 4.0f;          // mmHg - target right atrial pressure at rest (from Realheart paper)
    const float SUCTION_THRESHOLD_RV = 2.0f;  // mmHg - below this, reduce pump speed
    
    // Starling Mechanism RV (based on Realheart)
    // Maps RAP (preload) → target PAP (afterload)
    // From paper: RAP setpoint 4 mmHg, PAP increases with RAP
    const float STARLING_RV_BREAK1 = 2.0f;     // mmHg - first breakpoint
    const float STARLING_RV_BREAK2 = 6.0f;     // mmHg - second breakpoint
    const float STARLING_RV_BREAK3 = 12.0f;    // mmHg - third breakpoint
    const float STARLING_RV_VALUE1 = 12.0f;    // mmHg - target PAP at 2 mmHg RAP
    const float STARLING_RV_VALUE2 = 22.0f;    // mmHg - target PAP at 6 mmHg RAP
    const float STARLING_RV_MAX = 35.0f;       // mmHg - maximum target PAP
    const float STARLING_RV_SLOPE1 = 6.0f;     // 0-2 mmHg: target increase per mmHg RAP
    const float STARLING_RV_SLOPE2 = 2.5f;     // 2-6 mmHg: target increase per mmHg RAP
    const float STARLING_RV_SLOPE3 = 2.17f;    // 6-12 mmHg: target increase per mmHg RAP
    
    // ========== LEFT VENTRICLE (Pumps lungs → body) ==========
  
    const float MAX_LV_PRESSURE = 120.0f;      // Maximum pressure left ventricle can generate (mmHg)
    const float MIN_LV_PRESSURE = 0.0f;        // Minimum pressure (mmHg)
    const float LAP_SETPOINT = 9.0f;           // mmHg - target left atrial pressure at rest (from Realheart paper)
    const float SUCTION_THRESHOLD_LV = 2.0f;   // mmHg - below this, reduce pump speed
    
    // Frank-Starling Mechanism for Left Ventricle (based on Realheart paper)
    // Maps LAP (preload) → target AoP (afterload)
    // From paper: LAP setpoint 9 mmHg, AoP increases with LAP
    const float STARLING_LV_BREAK1 = 5.0f;     // mmHg - first breakpoint
    const float STARLING_LV_BREAK2 = 12.0f;    // mmHg - second breakpoint
    const float STARLING_LV_BREAK3 = 20.0f;    // mmHg - third breakpoint
    const float STARLING_LV_VALUE1 = 70.0f;    // mmHg - target AoP at 5 mmHg LAP
    const float STARLING_LV_VALUE2 = 100.0f;   // mmHg - target AoP at 12 mmHg LAP
    const float STARLING_LV_MAX = 120.0f;      // mmHg - maximum target AoP
    const float STARLING_LV_SLOPE1 = 14.0f;    // 0-5 mmHg: target increase per mmHg LAP
    const float STARLING_LV_SLOPE2 = 4.29f;    // 5-12 mmHg: target increase per mmHg LAP
    const float STARLING_LV_SLOPE3 = 2.5f;     // 12-20 mmHg: target increase per mmHg LAP
    
    // ========== CIRCULATION DYNAMICS ==========
    // How RAP and LAP change based on pump output and HR
    const float DEFAULT_RAP = RAP_SETPOINT;     // mmHg - RAP at rest
    const float DEFAULT_LAP = LAP_SETPOINT;     // mmHg - LAP at rest
    const float MIN_RAP = 2.0f;                // mmHg - minimum possible RAP
    const float MAX_RAP = 12.0f;               // mmHg - maximum possible RAP
    const float MIN_LAP = 5.0f;                // mmHg - minimum possible LAP
    const float MAX_LAP = 20.0f;               // mmHg - maximum possible LAP
    
    // Effect of AoP on RAP (venous return to right side)
    // From Realheart paper: AoP affects venous return
    const float AOP_EFFECT_ON_RAP = 0.05f;      // mmHg - RAP decreases by 0.05 per mmHg AoP
    const float PAP_EFFECT_ON_LAP = 0.08f;      // mmHg - LAP decreases by 0.08 per mmHg PAP
    const float HR_EFFECT_ON_VENOUS_RETURN = 0.05f;  // mmHg - increase per bpm above 72
    
    // ========== PUMP MOTOR CONSTANTS ==========
    const float MOTOR_MAX_RPM = 4720.0f;        // Maximum pump speed
    const float MOTOR_MAX_VOLTAGE = 12.0f;      // Maximum motor voltage (V)
    const float TAU = 0.25f;                    // Time constant for motor response (τ)(s)
    // First-order dynamics: motor speed follows setpoint with time constant τ
    // update: currentRPM += (setpointRPM - currentRPM) * (dt / τ)
    
    // ========== PRESSURE SENSOR CONSTANTS ==========
    const float SENSOR_CALIBRATION_ZERO = 0.093f;    // Voltage at 0 mmHg
    const float SENSOR_VOLTAGE_PER_MMHG = 0.0097f;   // Voltage increase per mmHg
    const float SENSOR_NOISE_LEVEL = 0.05f;          // Standard deviation of noise (mmHg)
    
    // ========== DEFAULT RPM CALCULATIONS ==========
    const float DEFAULT_RPM_RV = 3337.0f;
    const float DEFAULT_RPM_LV = 4021.0f;
    
    // ========== SIMULATION TIMING ==========
    const float DT = 0.05f;                    // Time step (seconds) - 50 ms
    const float SIM_DURATION = 20.0f;          // Total simulation time (seconds)
    const int TOTAL_ITERATIONS = static_cast<int>(SIM_DURATION / DT);
    const int SLEEP_MS = 50;                   // Sleep between iterations
    const int KEY_POLL_MS = 100;               // How often to check for key press
}