/**
 * @file PressureSensor.h
 * @brief Pressure sensor simulation with noise
 */

#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

class PressureSensor {
public:
    static constexpr float NOISE_LEVEL = 0.05f;
    
    PressureSensor();
    float measure(float truePressure);
    
private:
    static constexpr float MAX_PRESSURE = 100.0f;
};

#endif