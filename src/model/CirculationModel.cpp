/**
 * @file CirculationModel.cpp
 * @brief Models TRUE, REAL values we cant measure in this simulation
**/
#include "CirculationModel.h"
#include <algorithm>

constexpr float DEFAULT_HR = 72.0f;  // ≈4000 RPM
constexpr float DEFAULT_trueRAP = 4.0f;
constexpr float DEFAULT_trueLAP = 9.0f;

constexpr float MIN_trueRAP= 0.0f;
constexpr float MAX_trueRAP = 30.0f;

constexpr float MIN_trueLAP = 0.0f;
constexpr float MAX_trueLAP = 40.0f;

constexpr float AOP_EFFECT_ON_RAP = 0.05f;
constexpr float PAP_EFFECT_ON_LAP = 0.08f;
constexpr float HR_EFFECT_ON_VENOUS_RETURN = 0.05f;

float updateTrueRAP(float trueAOP, float HR) {
    float aopEffect = trueAOP * AOP_EFFECT_ON_RAP;
    float rap = DEFAULT_trueRAP - aopEffect;
    rap = std::max(MIN_trueRAP, std::min(MAX_trueRAP, rap));
    rap += (HR - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    return rap;
}

float updateTrueLAP(float truePAP, float HR) {
    float papEffect = truePAP * PAP_EFFECT_ON_LAP;
    float lap = DEFAULT_trueLAP - papEffect;
    lap = std::max(MIN_trueLAP, std::min(MAX_trueLAP, lap));
    lap += (HR - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    return lap;
}