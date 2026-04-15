#include "Motor.h"
#include <algorithm>



Motor::Motor(float maxFlow)
    : m_maxFlow(maxFlow)
    , m_setpointRPM(0.0f)
    , m_currentRPM(0.0f)
    , m_currentVoltage(0.0f) {}

void Motor::initialize(float startRPM) {
    m_setpointRPM = startRPM;
    m_currentRPM = startRPM;
    m_currentVoltage = rpmToVoltage(startRPM);
}

float Motor::flowToRPM(float flow) const {
    float rpm = (flow / m_maxFlow) * MAX_RPM;
    return std::max(0.0f, std::min(MAX_RPM, rpm));
}

float Motor::rpmToFlow(float rpm) const {
    return (rpm / MAX_RPM) * m_maxFlow;
}

float Motor::rpmToVoltage(float rpm) const {
    return (rpm / MAX_RPM) * MAX_VOLTAGE;
}

void Motor::setTargetFlow(float flow) {
    m_setpointRPM = flowToRPM(flow);
}

void Motor::update(float dt) {
    float diff = m_setpointRPM - m_currentRPM;
    float gain = dt / TAU;
    m_currentRPM += diff * gain;
    m_currentRPM = std::max(0.0f, std::min(MAX_RPM, m_currentRPM));
    m_currentVoltage = rpmToVoltage(m_currentRPM);
}

float Motor::getActualFlow() const { return rpmToFlow(m_currentRPM); }
float Motor::getActualRPM() const { return m_currentRPM; }
float Motor::getActualVoltage() const { return m_currentVoltage; }
float Motor::getSetpointRPM() const { return m_setpointRPM; }

void Motor::reduceSpeed(float reductionFactor) {
    m_setpointRPM *= reductionFactor;
    m_currentRPM *= reductionFactor;
}