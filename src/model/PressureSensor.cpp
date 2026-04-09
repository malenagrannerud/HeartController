#include "PressureSensor.h"
#include <algorithm>

PressureSensor::PressureSensor(float maxPressure) 
    : m_maxPressure(maxPressure) {}

float PressureSensor::measure(float truePressure) {
    return std::max(0.0f, std::min(m_maxPressure, truePressure));
}

float PressureSensor::pressureToVoltage(float pressure) const {
    return CALIBRATION_ZERO + pressure * VOLTAGE_PER_MMHG;
}