
/**
 * @file frank_starling.cpp
 * @brief ISO 62304: Implementation av Frank-Starling mekanism
 * @version 1.0.0
 */

#include "../include/frank_starling.h"
#include <algorithm>
#include <cassert>

FrankStarling::FrankStarling(uint32_t version) noexcept
    : m_version(version)
    , m_calls(0)
    , m_last_input(0.0)
    , m_last_output(0.0)
    , m_last_error(ErrorCode::FS_SUCCESS)
{
    // ISO 62304 Section 5.5: Verifiering av konstanter
    static_assert(NUM_POINTS == 4, "Invalid number of curve points");
    static_assert(MIN_PRELOAD <= MAX_PRELOAD, "Invalid preload range");
    static_assert(MIN_AFTERLOAD <= MAX_AFTERLOAD, "Invalid afterload range");
}

bool FrankStarling::validateInput(double inletPressure) const noexcept {
    if (inletPressure < MIN_PRELOAD || inletPressure > MAX_PRELOAD) {
        const_cast<FrankStarling*>(this)->m_last_error = ErrorCode::FS_INVALID_PRELOAD;
        return false;
    }
    return true;
}

double FrankStarling::linearInterpolate(double x, double x1, double y1, double x2, double y2) const noexcept {
    if (x2 == x1) return y1;
    double t = (x - x1) / (x2 - x1);
    return y1 + t * (y2 - y1);
}

double FrankStarling::calculateTargetPressure(double inletPressure) noexcept {
    // ISO 62304 Section 5.6: Spårbarhet - logga varje anrop
    m_calls++;
    m_last_input = inletPressure;
    
    // Verifiering av inparametrar
    if (!validateInput(inletPressure)) {
        m_last_output = 0.0;
        return 0.0;
    }
    
    double result;
    
    // SRS-FS-002: Hantera mättnad
    if (inletPressure <= PRELOAD_POINTS[0]) {
        result = AFTERLOAD_POINTS[0];
        m_last_error = ErrorCode::FS_SUCCESS;
    }
    else if (inletPressure >= PRELOAD_POINTS[NUM_POINTS - 1]) {
        result = AFTERLOAD_POINTS[NUM_POINTS - 1];
        if (inletPressure > PRELOAD_POINTS[NUM_POINTS - 1] + 0.1) {
            m_last_error = ErrorCode::FS_SATURATION_WARNING;
        } else {
            m_last_error = ErrorCode::FS_SUCCESS;
        }
    }
    else {
        // SRS-FS-001: Interpolera mellan punkter
        for (int i = 1; i < NUM_POINTS; i++) {
            if (inletPressure <= PRELOAD_POINTS[i]) {
                result = linearInterpolate(inletPressure,
                    PRELOAD_POINTS[i-1], AFTERLOAD_POINTS[i-1],
                    PRELOAD_POINTS[i], AFTERLOAD_POINTS[i]);
                m_last_error = ErrorCode::FS_SUCCESS;
                break;
            }
        }
    }
    
    // SRS-FS-003: Begränsa utdata till fysiologiskt område
    result = std::max(MIN_AFTERLOAD, std::min(MAX_AFTERLOAD, result));
    m_last_output = result;
    
    // Spårbarhet (ISO 62304 Section 5.4)
    assert(result >= MIN_AFTERLOAD && result <= MAX_AFTERLOAD);
    
    return result;
}

void FrankStarling::resetTraceability() noexcept {
    m_calls = 0;
    m_last_input = 0.0;
    m_last_output = 0.0;
    m_last_error = ErrorCode::FS_SUCCESS;
}
