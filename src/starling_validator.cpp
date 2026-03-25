#include "../include/starling_validator.h"
#include <algorithm>

bool StarlingValidator::validatePoint(double preload, double afterload) noexcept {
    return (preload >= MIN_PRELOAD && preload <= MAX_PRELOAD &&
            afterload >= MIN_AFTERLOAD && afterload <= MAX_AFTERLOAD);
}

bool StarlingValidator::isMonotonic(const double preload[], const double afterload[], 
                                     size_t size) noexcept {
    if (size < 2) return false;
    
    for (size_t i = 1; i < size; i++) {
        if (preload[i] <= preload[i-1]) return false;
        if (afterload[i] < afterload[i-1]) return false;
    }
    return true;
}

bool StarlingValidator::withinLimits(const double preload[], const double afterload[], 
                                      size_t size) noexcept {
    for (size_t i = 0; i < size; i++) {
        if (!validatePoint(preload[i], afterload[i])) return false;
    }
    return true;
}

bool StarlingValidator::hasSaturation(const double afterload[], size_t size, double maxPressure) noexcept {
    if (size == 0) return false;
    return (afterload[size - 1] >= maxPressure - 0.1);
}

double StarlingValidator::getGain(const double preload[], const double afterload[], 
                                   size_t size, double inletPressure) noexcept {
    if (size < 2) return 0.0;
    if (inletPressure <= preload[0]) return 0.0;
    if (inletPressure >= preload[size - 1]) return 0.0;
    
    for (size_t i = 1; i < size; i++) {
        if (inletPressure <= preload[i]) {
            return (afterload[i] - afterload[i-1]) / (preload[i] - preload[i-1]);
        }
    }
    return 0.0;
}

bool StarlingValidator::validateCurve(const double preload[], const double afterload[], 
                                       size_t size, ErrorCode& error) noexcept {
    if (size < MIN_POINTS) {
        error = ErrorCode::CV_INVALID_POINTS;
        return false;
    }
    
    if (!withinLimits(preload, afterload, size)) {
        error = ErrorCode::CV_OUT_OF_RANGE;
        return false;
    }
    
    if (!isMonotonic(preload, afterload, size)) {
        error = ErrorCode::CV_NON_MONOTONIC;
        return false;
    }
    
    error = ErrorCode::FS_SUCCESS;
    return true;
}
