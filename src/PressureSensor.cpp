#include "PressureSensor.h"
#include <random>
#include <algorithm>

PressureSensor::PressureSensor(float maxPressure) 
    : m_maxPressure(maxPressure) {}

float PressureSensor::measure(float truePressure) {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    static std::normal_distribution<float> noise(0, Constants::SENSOR_NOISE_LEVEL);
    
    float noisyPressure = truePressure + noise(gen);
    return std::max(0.0f, std::min(m_maxPressure, noisyPressure));
}

float PressureSensor::pressureToVoltage(float pressure) const {
    return Constants::SENSOR_CALIBRATION_ZERO + pressure * Constants::SENSOR_VOLTAGE_PER_MMHG;
}
