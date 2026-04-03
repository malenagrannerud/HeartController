#ifndef STARLING_CURVE_H
#define STARLING_CURVE_H

#include <vector>
#include <utility>

class StarlingCurve {
public:
    static std::vector<std::pair<float, float>> getDefaultRVPoints();
    static std::vector<std::pair<float, float>> getDefaultLVPoints();
    
    explicit StarlingCurve(const std::vector<std::pair<float, float>>& points);
    float evaluate(float preload) const;
    
private:
    std::vector<std::pair<float, float>> m_breakpoints;
};

#endif
