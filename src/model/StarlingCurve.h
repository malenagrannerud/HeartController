/**
 * @file StarlingCurve.h
 * @brief Frank-Starling curve. Maps 
 *    - RAP [mmHg] to target CO_RV [L/min] and 
 *    - LAP [mmHg] to target CO_LV [L/min].
 * 
 * Higher RAP/LAP (Preload) -> stronger contraction -> higher CO (Cardiac Output).
 * 
 * X-axis: Preload (RAP/LAP) in mmHg
 * Y-axis: Cardiac Output (CO) in L/min (calibrated for REFERENCE_HR)
 */

#ifndef STARLING_CURVE_H  
#define STARLING_CURVE_H

#include <vector>   
#include <utility>  

class StarlingCurve {
public:
    static std::vector<std::pair<float, float>> getRVPoints();
    static std::vector<std::pair<float, float>> getLVPoints();
    
    explicit StarlingCurve(const std::vector<std::pair<float, float>>& points);
    
    /**
     * @brief Get cardiac output for given preload at specified heart rate
     * @param preload Atrial filling pressure (mmHg)
     * @param hr Heart rate (bpm)
     * @return Cardiac output (L/min)
     */
    float evaluate(float preload, float hr) const;
    
private:
    std::vector<std::pair<float, float>> m_points;
};

#endif
