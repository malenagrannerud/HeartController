/**
 * @file PressureSensor.h
 * @brief Pressure sensor simulation with noise
 */

#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

/**
 * @brief Simulates a pressure sensor with Gaussian noise
 * 
 * Adds random noise to the true pressure value to simulate
 * real-world sensor measurements.
 */
class PressureSensor {
public:
    /**
     * @brief Constructor
     */
    PressureSensor();

    /**
     * @brief Measure pressure with added noise
     * @param truePressure The true pressure value (mmHg)
     * @return Measured pressure with noise (mmHg)
     */
    float measure(float truePressure);
};

#endif