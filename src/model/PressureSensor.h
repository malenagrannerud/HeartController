
/**
 * @file PressureSensor.h
 * @brief Pressure sensor simulation with noise
 */

#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

/**
 * @brief Simulates a pressure sensor with Gaussian noise
 */
class PressureSensor {
public:
    // ========== Sensor Constants ==========
    static constexpr float CALIBRATION_ZERO = 0.093f;     // Voltage at 0 mmHg (V)
    static constexpr float VOLTAGE_PER_MMHG = 0.0097f;    // Voltage increase per mmHg (V/mmHg)
    static constexpr float NOISE_LEVEL = 0.05f;           // Standard deviation of noise (mmHg)
    
    explicit PressureSensor(float maxPressure);
    float measure(float truePressure);
    float pressureToVoltage(float pressure) const;
    
private:
    float m_maxPressure;
};

#endif