#include "StarlingMechanism.h"
#include "Constants.h"

float starlingRight(float rap) {
    using namespace Constants;
    
    if (rap <= 0) return 0;
    if (rap <= STARLING_RV_BREAK1) {
        return rap * STARLING_RV_SLOPE1;
    }
    if (rap <= STARLING_RV_BREAK2) {
        return STARLING_RV_VALUE1 + (rap - STARLING_RV_BREAK1) * STARLING_RV_SLOPE2;
    }
    if (rap <= STARLING_RV_BREAK3) {
        return STARLING_RV_VALUE2 + (rap - STARLING_RV_BREAK2) * STARLING_RV_SLOPE3;
    }
    return STARLING_RV_MAX;
}

float starlingLeft(float lap) {
    using namespace Constants;
    
    if (lap <= 0) return 0;
    if (lap <= STARLING_LV_BREAK1) {
        return lap * STARLING_LV_SLOPE1;
    }
    if (lap <= STARLING_LV_BREAK2) {
        return STARLING_LV_VALUE1 + (lap - STARLING_LV_BREAK1) * STARLING_LV_SLOPE2;
    }
    if (lap <= STARLING_LV_BREAK3) {
        return STARLING_LV_VALUE2 + (lap - STARLING_LV_BREAK2) * STARLING_LV_SLOPE3;
    }
    return STARLING_LV_MAX;
}