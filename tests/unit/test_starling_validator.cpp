#include <iostream>
#include "../../include/starling_validator.h"

int tests_passed = 0;
int tests_failed = 0;

void assert_true(bool condition, const char* name) {
    std::cout << "  " << name << ": ";
    if (condition) {
        tests_passed++;
        std::cout << "✓ PASSED" << std::endl;
    } else {
        tests_failed++;
        std::cout << "✗ FAILED" << std::endl;
    }
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "StarlingValidator - Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Test validatePoint
    std::cout << "\nTest: validatePoint()" << std::endl;
    assert_true(StarlingValidator::validatePoint(5.0, 22.5), "Valid (5, 22.5)");
    assert_true(StarlingValidator::validatePoint(0.0, 0.0), "Valid (0, 0)");
    
    // Test isMonotonic
    std::cout << "\nTest: isMonotonic()" << std::endl;
    double preload[] = {0, 2, 10, 15};
    double afterload[] = {0, 18, 30, 30};
    assert_true(StarlingValidator::isMonotonic(preload, afterload, 4), "Valid monotonic");
    
    // Test withinLimits
    std::cout << "\nTest: withinLimits()" << std::endl;
    assert_true(StarlingValidator::withinLimits(preload, afterload, 4), "All within limits");
    
    // Test validateCurve
    std::cout << "\nTest: validateCurve()" << std::endl;
    ErrorCode error;
    assert_true(StarlingValidator::validateCurve(preload, afterload, 4, error), "Valid curve");
    
    // Test hasSaturation
    std::cout << "\nTest: hasSaturation()" << std::endl;
    assert_true(StarlingValidator::hasSaturation(afterload, 4, 30.0), "Saturation detected");
    
    // Test getGain
    std::cout << "\nTest: getGain()" << std::endl;
    double gain = StarlingValidator::getGain(preload, afterload, 4, 5.0);
    assert_true(gain > 1.4 && gain < 1.6, "Gain at 5 mmHg ≈ 1.5");
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}
