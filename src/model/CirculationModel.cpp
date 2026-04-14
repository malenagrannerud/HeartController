/**
 * @file CirculationModel.cpp
 * @brief Implementation of circulation dynamics for preload based control
 * Models how RAP and LAP change in response to actual AoP and PAP



#include "CirculationModel.h"
#include <algorithm>

constexpr float DEFAULT_RAP = 4.0f;
constexpr float MIN_RAP = 2.0f;
constexpr float MAX_RAP = 15.0f;


constexpr float DEFAULT_LAP = 9.0f;
constexpr float MIN_LAP = 6.0f;
constexpr float MAX_LAP = 20.0f;

constexpr float DEFAULT_HR = 72.0f;

constexpr float AOP_EFFECT_ON_RAP = 0.05f;
constexpr float PAP_EFFECT_ON_LAP = 0.08f;
constexpr float HR_EFFECT_ON_VENOUS_RETURN = 0.05f;



float calculateNewRAP(float actualAoP, float heartRate) {
    float aopEffect = actualAoP * AOP_EFFECT_ON_RAP;
    float rap = DEFAULT_RAP - aopEffect;
    rap = std::max(MIN_RAP, std::min(MAX_RAP, rap));
    rap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    return rap;
}

float calculateNewLAP(float actualPAP, float heartRate) {
    float papEffect = actualPAP * PAP_EFFECT_ON_LAP;
    float lap = DEFAULT_LAP - papEffect;
    lap = std::max(MIN_LAP, std::min(MAX_LAP, lap));
    lap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    return lap;
}