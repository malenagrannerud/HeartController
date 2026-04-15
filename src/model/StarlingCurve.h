/**
 * @file StarlingCurve.h
 * @brief Frank-Starling curve - maps preload (atrial pressure) to target afterload
 * 
 * The Frank-Starling mechanism: higher filling pressure → stronger contraction → higher stroke volume.
 * This class provides piecewise linear interpolation between defined breakpoints.
 */

#ifndef STARLING_CURVE_H  // Header guard - prevents double inclusion
#define STARLING_CURVE_H

#include <vector>   // has std::vector - stores the curve breakpoints
#include <utility>  // has std::pair - each point is (preload, targetPressure)

class StarlingCurve {
public:
    /**
     * @return Default points RV: RAP (mmHg) → target PAP (mmHg)
     */
    static std::vector<std::pair<float, float>> getDefaultRVPoints();
    
    /**
     * @return Default points LV: LAP (mmHg) → target AoP (mmHg)
     */
    static std::vector<std::pair<float, float>> getDefaultLVPoints();
    
    /**
     * @brief Construct a curve from breakpoints
     * @param points Vector of (preload, targetPressure) pairs - sorted automatically
     */
    explicit StarlingCurve(const std::vector<std::pair<float, float>>& points);
    
    /**
     * @brief Evaluate the curve at a given preload
     * @param preload Atrial filling pressure (mmHg)
     * @return Target afterload pressure (mmHg) via linear interpolation
     */
    float evaluate(float preload) const;
    
private:
    std::vector<std::pair<float, float>> m_breakpoints;  // Sorted (x, y) points
};

#endif  // STARLING_CURVE_H