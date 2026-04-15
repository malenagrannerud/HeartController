#include "CirculationModel.h"
#include <algorithm>

CirculationModel::CirculationModel()
    : m_hr(DEFAULT_HR)
    , m_rap(DEFAULT_RAP)
    , m_lap(DEFAULT_LAP)
    , m_pap(DEFAULT_PAP)
    , m_aop(DEFAULT_AOP)
    , m_coRV(5.0f)
    , m_coLV(5.0f) {}

void CirculationModel::reset() {
    m_hr = DEFAULT_HR;
    m_rap = DEFAULT_RAP;
    m_lap = DEFAULT_LAP;
    m_pap = DEFAULT_PAP;
    m_aop = DEFAULT_AOP;
    m_coRV = 5.0f;
    m_coLV = 5.0f;
}

void CirculationModel::update(float hr, float coRV, float coLV) {
    m_hr = hr;
    m_coRV = coRV;
    m_coLV = coLV;
    
    m_pap = (coRV * PULMONARY_RESISTANCE) / 80.0f;
    m_aop = (coLV * SYSTEMIC_RESISTANCE) / 80.0f;
    
    float hrEffect = (m_hr - DEFAULT_HR) * HR_EFFECT_ON_PRELOAD;
    
    m_rap = DEFAULT_RAP - (m_aop * AOP_EFFECT_ON_RAP) + hrEffect;
    m_rap = clamp(m_rap, MIN_RAP, MAX_RAP);
    
    m_lap = DEFAULT_LAP - (m_pap * PAP_EFFECT_ON_LAP) + hrEffect;
    m_lap = clamp(m_lap, MIN_LAP, MAX_LAP);
}

float CirculationModel::clamp(float value, float min, float max) const {
    return std::max(min, std::min(max, value));
}

float CirculationModel::getRAP() const { return m_rap; }
float CirculationModel::getLAP() const { return m_lap; }
float CirculationModel::getPAP() const { return m_pap; }
float CirculationModel::getAoP() const { return m_aop; }
float CirculationModel::getCO_RV() const { return m_coRV; }
float CirculationModel::getCO_LV() const { return m_coLV; }

float CirculationModel::getBalance() const {
    if (m_coLV > 0) return m_coRV / m_coLV;
    return 1.0f;
}