
/**
 * @file PressureSensor.h
 * @brief Pressure sensor simulation with noise
 */

#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

class PressureSensor {
public:
    static constexpr float CALIBRATION_ZERO = 0.093f;
    static constexpr float VOLTAGE_PER_MMHG = 0.0097f;
    static constexpr float NOISE_LEVEL = 0.05f;
    
    explicit PressureSensor(float maxPressure);
    float measure(float truePressure);
    float pressureToVoltage(float pressure) const;
    
private:
    float m_maxPressure;
};

#endif