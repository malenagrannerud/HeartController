#include "../include/pressure_sensor.h"
#include <random>
#include <algorithm>

PressureSensor::PressureSensor() {}

float PressureSensor::measure(float truePressure) {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    static std::normal_distribution<float> noise(0, 0.1);
    
    float noisy = truePressure + noise(gen);
    return std::max(0.0f, std::min(40.0f, noisy));
}