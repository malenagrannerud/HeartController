#include "Motor.h"
#include <algorithm>
#include <cmath>

Motor::Motor(float maxPressure)
    : m_maxPressure(maxPressure)
    , m_setpointRPM(0)
    , m_currentRPM(0)
    , m_currentVoltage(0) {}

void Motor::initialize(float defaultRPM) {
    m_currentRPM = defaultRPM;
    m_setpointRPM = defaultRPM;
    m_currentVoltage = rpmToVoltage(m_currentRPM);
}

float Motor::pressureToRPM(float pressure) const {
    float normalizedPressure = pressure / m_maxPressure;
    float rpm = std::sqrt(normalizedPressure) * MAX_RPM;
    return std::max(0.0f, std::min(static_cast<float>(MAX_RPM), rpm));
}

float Motor::rpmToPressure(float rpm) const {
    float normalizedRPM = rpm / MAX_RPM;
    float pressure = m_maxPressure * normalizedRPM * normalizedRPM;
    return std::min(m_maxPressure, pressure);
}

float Motor::rpmToVoltage(float rpm) const {
    return (rpm / MAX_RPM) * MAX_VOLTAGE;
}

void Motor::setTargetPressure(float targetPressure) {
    m_setpointRPM = pressureToRPM(targetPressure);
}

float Motor::getSetpointRPM() const { 
    return m_setpointRPM; 
}

float Motor::getSetpointVoltage() const { 
    return rpmToVoltage(m_setpointRPM); 
}

void Motor::update(float dt) {
    float diff = m_setpointRPM - m_currentRPM;
    float gain = dt / TAU;
    m_currentRPM += diff * gain;
    m_currentRPM = std::max(0.0f, std::min(static_cast<float>(MAX_RPM), m_currentRPM));
    m_currentVoltage = rpmToVoltage(m_currentRPM);
}

float Motor::getActualPressure() const { 
    return rpmToPressure(m_currentRPM); 
}

float Motor::getActualRPM() const { 
    return m_currentRPM; 
}

float Motor::getActualVoltage() const { 
    return m_currentVoltage; 
}

void Motor::reduceSpeed(float reductionFactor) {
    m_setpointRPM *= reductionFactor;
    m_currentRPM *= reductionFactor;
}

float Motor::getMaxPressure() const { 
    return m_maxPressure; 
}