#include "../include/starling_algorithm.h"

FrankStarling::FrankStarling() {
    numPoints = 4;
    preloadPoints[0] = 0; afterloadPoints[0] = 0;
    preloadPoints[1] = 2; afterloadPoints[1] = 18;
    preloadPoints[2] = 10; afterloadPoints[2] = 30;
    preloadPoints[3] = 15; afterloadPoints[3] = 30;
}

float FrankStarling::calculateTargetPressure(float inletPressure) {
    if (inletPressure <= preloadPoints[0]) return afterloadPoints[0];
    if (inletPressure >= preloadPoints[numPoints - 1]) return afterloadPoints[numPoints - 1];
    
    for (int i = 1; i < numPoints; i++) {
        if (inletPressure <= preloadPoints[i]) {
            float slope = (afterloadPoints[i] - afterloadPoints[i-1]) / 
                          (preloadPoints[i] - preloadPoints[i-1]);
            return afterloadPoints[i-1] + slope * (inletPressure - preloadPoints[i-1]);
        }
    }
    return afterloadPoints[numPoints - 1];
}