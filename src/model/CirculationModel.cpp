#include "CirculationModel.h"
#include <algorithm>
#include <cmath>

CirculationModel::CirculationModel()
    : m_hr(DEFAULT_HR)
    , m_rap(DEFAULT_RAP)
    , m_lap(DEFAULT_LAP)
    , m_pap(DEFAULT_PAP)
    , m_aop(DEFAULT_AOP) {
}

void CirculationModel::reset() {
    m_hr = DEFAULT_HR;
    m_rap = DEFAULT_RAP;
    m_lap = DEFAULT_LAP;
    m_pap = DEFAULT_PAP;
    m_aop = DEFAULT_AOP;
}

void CirculationModel::update(float hr, float actualPAP, float actualAoP) {
    m_hr = hr;
    m_pap = actualPAP;
    m_aop = actualAoP;
    
    float hrEffect = (m_hr - DEFAULT_HR) * HR_EFFECT_ON_PRELOAD;
    
    float aopEffect = m_aop * AOP_EFFECT_ON_RAP;
    m_rap = DEFAULT_RAP - aopEffect + hrEffect;
    m_rap = clamp(m_rap, MIN_RAP, MAX_RAP);
    
    float papEffect = m_pap * PAP_EFFECT_ON_LAP;
    m_lap = DEFAULT_LAP - papEffect + hrEffect;
    m_lap = clamp(m_lap, MIN_LAP, MAX_LAP);
}

float CirculationModel::clamp(float value, float min, float max) const {
    return std::max(min, std::min(max, value));
}

float CirculationModel::getRAP() const { return m_rap; }
float CirculationModel::getLAP() const { return m_lap; }
float CirculationModel::getPAP() const { return m_pap; }
float CirculationModel::getAoP() const { return m_aop; }

float CirculationModel::getCO_RV() const {
    // CO = (PAP / PVR) * 80
    return (m_pap * 80.0f) / PULMONARY_RESISTANCE;
}

float CirculationModel::getCO_LV() const {
    // CO = (AoP / SVR) * 80
    return (m_aop * 80.0f) / SYSTEMIC_RESISTANCE;
}

float CirculationModel::getCO() const {
    return (getCO_RV() + getCO_LV()) / 2.0f;
}

float CirculationModel::getBalance() const {
    float coLV = getCO_LV();
    if (coLV > 0) {
        return getCO_RV() / coLV;
    }
    return 1.0f;
}