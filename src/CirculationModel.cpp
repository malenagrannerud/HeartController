#include "CirculationModel.h"
#include "Constants.h"
#include <algorithm>

float calculateNewRAP(float actualAoP, float heartRate) {
    using namespace Constants;
    
    float aopEffect = actualAoP * AOP_EFFECT_ON_RAP;
    float rap = DEFAULT_RAP - aopEffect;
    rap = std::max(MIN_RAP, std::min(MAX_RAP, rap));
    rap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    
    return rap;
}

float calculateNewLAP(float actualPAP, float heartRate) {
    using namespace Constants;
    
    float papEffect = actualPAP * PAP_EFFECT_ON_LAP;
    float lap = DEFAULT_LAP - papEffect;
    lap = std::max(MIN_LAP, std::min(MAX_LAP, lap));
    lap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    
    return lap;
}