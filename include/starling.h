#ifndef STARLING_ALGORITHM_H
#define STARLING_ALGORITHM_H

class FrankStarling {
private:
    float preloadPoints[4];
    float afterloadPoints[4];
    int numPoints;
    
public:
    FrankStarling();
    float calculateTargetPressure(float inletPressure);
};

#endif