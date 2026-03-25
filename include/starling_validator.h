
/**
 * @file starling_validator.h
 * @brief ISO 62304: Validering av Starling kurva
 * @version 1.0.0
 * 
 * Validerar att Starling kurvan uppfyller medicinska krav:
 * - Monotont ökande
 * - Inom fysiologiska gränser
 * - Tillräckligt antal punkter
 */

#ifndef STARLING_VALIDATOR_H
#define STARLING_VALIDATOR_H

#include <cstddef>
#include "error_codes.h"

class StarlingValidator {
public:
    static constexpr double MIN_PRELOAD = 0.0;
    static constexpr double MAX_PRELOAD = 40.0;
    static constexpr double MIN_AFTERLOAD = 0.0;
    static constexpr double MAX_AFTERLOAD = 30.0;
    static constexpr size_t MIN_POINTS = 2;
    
    static bool validateCurve(const double preload[], const double afterload[], 
                              size_t size, ErrorCode& error) noexcept;
    
    static bool isMonotonic(const double preload[], const double afterload[], 
                            size_t size) noexcept;
    
    static bool withinLimits(const double preload[], const double afterload[], 
                             size_t size) noexcept;
    
    static bool validatePoint(double preload, double afterload) noexcept;
    
    static bool hasSaturation(const double afterload[], size_t size, double maxPressure) noexcept;
    
    static double getGain(const double preload[], const double afterload[], 
                          size_t size, double inletPressure) noexcept;
};

#endif
