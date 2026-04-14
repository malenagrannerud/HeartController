/**
 @file CirculationModel.cpp

*/

#include "CirculationModel.h"
#include <algorithm>

constexpr float DEFAULT_HR = 72.0f;
constexpr float DEFAULT_RAP = 4.0f;
constexpr float DEFAULT_LAP = 9.0f;

constexpr float MIN_RAP = 0.0f;     // <2 is patological, but we allow it to go to 0 for testing and extreme cases
constexpr float MAX_RAP = 30.0f;    // >15 is patological, but we allow it to go higher for testing and extreme cases

constexpr float MIN_LAP = 0.0f;     // <6 patological, but allowed here 0 for testing and extreme cases
constexpr float MAX_LAP = 40.0f;    // >25 patological, but allowed here 0 for testing and extreme cases

constexpr float AOP_EFFECTS_RAP = 0.02f;   // If AoP rises → 2 mmHg minskning i RAP
constexpr float PAP_EFFECTS_LAP = 0.03f;   // 30 mmHg PAP → 0.9 mmHg minskning i LAP
constexpr float HR_EFFECTS_RAP_OR_LAP = 0.05f;  // 50 bpm ökning → 2.5 mmHg ökning



float updateRAP(float actualAoP, float HR) {
    float aopEffect = actualAoP * AOP_EFFECTS_RAP;
    float rap = DEFAULT_RAP - aopEffect;
    rap = std::max(MIN_RAP, std::min(MAX_RAP, rap));
    rap += (HR - DEFAULT_HR) * HR_EFFECTS_RAP_OR_LAP;
    return rap;
}

float updateLAP(float actualPAP, float HR) {
    float papEffect = actualPAP * PAP_EFFECTS_LAP;
    float lap = DEFAULT_LAP - papEffect;
    lap = std::max(MIN_LAP, std::min(MAX_LAP, lap));
    lap += (HR - DEFAULT_HR) * HR_EFFECTS_RAP_OR_LAP;
    return lap;
}