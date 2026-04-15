#include "PressureSensor.h"
#include <random>
#include <algorithm>

PressureSensor::PressureSensor() {}

float PressureSensor::measure(float truePressure) {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
    static std::normal_distribution<float> noise(0.0f, NOISE_LEVEL);
    
    float noisyPressure = truePressure + noise(gen);
    return std::max(0.0f, std::min(MAX_PRESSURE, noisyPressure));
}