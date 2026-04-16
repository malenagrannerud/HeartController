/**
 * @file test_starling.cpp
 * @brief Unit tests for StarlingCurve.cpp
 * 
 *
 * bash:   cd /workspaces/HeartController/build && make test_starling && ./bin/test_starling
 */

#include "../src/model/StarlingCurve.h" 
#include <cassert>  
#include <iostream>     
#include <cmath>        

#define TEST(name) void name() 
#define ASSERT_NEAR(a, b, eps) \
    if (std::abs((a) - (b)) >= (eps)) { \
        std::cerr << "❌ FAIL: " << #a << " (" << a << ") is not near " << b << std::endl; \
        assert(false); \
    }
#define ASSERT_TRUE(cond) \
    if (!(cond)) { \
        std::cerr << "❌ FAIL: Condition failed: " << #cond << std::endl; \
        assert(false); \
    }


/**
 * @test test_curves_not_empty
 * 
 * Test that predefined curves return non-zero values at normal preload.
 */
TEST(test_curves_not_empty) {                       
    StarlingCurve rv(StarlingCurve::getRVPoints());  
    StarlingCurve lv(StarlingCurve::getLVPoints());
    ASSERT_TRUE(rv.evaluate(5.0f, 72.0f) > 0.0f);
    ASSERT_TRUE(lv.evaluate(5.0f, 72.0f) > 0.0f);
    std::cout << "✅ Pre-defined curves not empty\n";
}



/**
 * @test test_rv_curve
 * 
 * Test RV curve specific points (based on getRVPoints logic).
 */
TEST(test_rv_curve) {
    StarlingCurve rv(StarlingCurve::getRVPoints());

    // Testing points: {0,0}, {2,4}, {4,5}, {7,6.5}, {12,8}
    ASSERT_NEAR(rv.evaluate(0.0f, 72.0f), 0.0f, 0.1f);
    ASSERT_NEAR(rv.evaluate(2.0f, 72.0f), 40.0f, 0.1f);
    ASSERT_NEAR(rv.evaluate(6.0f, 72.0f), 60.0f, 0.1f);
    ASSERT_NEAR(rv.evaluate(12.0f, 72.0f), 70.0f, 0.1f);
    std::cout << "✅ RV curve values correct\n";
}




/**
 * @test test_rv_edge_cases
 * 
 * Test edge cases: negative preload, above max, and HR scaling.
 */
TEST(test_rv_edge_cases) {
    StarlingCurve rv(StarlingCurve::getRVPoints());
    
    // Negative preload - should handle as 0
    ASSERT_NEAR(rv.evaluate(-5.0f, 72.0f), 0.0f, 0.01f);
    
    // Preload above max (12.0) - should return plateau (8.0)
    ASSERT_NEAR(rv.evaluate(20.0f, 72.0f), 8.0f, 0.01f);
    
    // HR Scaling: Doubling HR should double CO
    float co_72 = rv.evaluate(4.0f, 72.0f);
    float co_144 = rv.evaluate(4.0f, 144.0f);
    ASSERT_NEAR(co_144, co_72 * 2.0f, 0.01f);
    
    std::cout << "✅ RV edge cases and HR scaling correct\n";
}



/**
 * Test LV curve specific points.
 */
TEST(test_lv_curve) {
    StarlingCurve lv(StarlingCurve::getLVPoints());
    // Testing points: {0,0}, {5,3.5}, {8,5}, {14,6.5}, {22,8}
    ASSERT_NEAR(lv.evaluate(0.0f, 72.0f), 0.0f, 0.01f);
    ASSERT_NEAR(lv.evaluate(8.0f, 72.0f), 5.0f, 0.01f);
    ASSERT_NEAR(lv.evaluate(22.0f, 72.0f), 8.0f, 0.01f);
    std::cout << "✅ LV curve values correct\n";
}





/**
 * @test test_lv_edge_cases
 * 
 * Tests edge cases for LV curve.
 */
TEST(test_lv_edge_cases) {
    StarlingCurve lv(StarlingCurve::getLVPoints());
    
    // Negative preload
    ASSERT_NEAR(lv.evaluate(-5.0f), 0.0f, 0.1f);
    
    // Preload above max
    ASSERT_NEAR(lv.evaluate(30.0f), 90.0f, 0.1f);
    ASSERT_NEAR(lv.evaluate(100.0f), 90.0f, 0.1f);
    
    // Interpolation
    float sv_at_8 = lv.evaluate(8.0f);
    ASSERT_TRUE(sv_at_8 > 50.0f && sv_at_8 < 75.0f);
    
    std::cout << "✅ LV edge cases handled correctly\n";
}




/**
 * @test test_monotonic
 * 
 * Curve should be monotonically increasing (never decreases).
 */
TEST(test_monotonic) {
    StarlingCurve rv(StarlingCurve::getRVPoints());
    StarlingCurve lv(StarlingCurve::getLVPoints());
    
    // Test RV curve
    float prev = rv.evaluate(0.0f);
    for (float preload = 0.5f; preload < 25.0f; preload += 0.5f) {
        float current = rv.evaluate(preload);
        ASSERT_TRUE(current >= prev);
        prev = current;
    }
    
    // Test LV curve
    prev = lv.evaluate(0.0f);
    for (float preload = 0.5f; preload < 25.0f; preload += 0.5f) {
        float current = lv.evaluate(preload);
        ASSERT_TRUE(current >= prev);
        prev = current;
    }
    
    std::cout << "✅ Curves are monotonically increasing\n";
}



int main() {
    std::cout << "Running StarlingCurve tests...\n\n";
   
    test_curves_not_empty();
    test_rv_curve();
    test_rv_edge_cases();
    test_lv_curve();
    test_lv_edge_cases();
    test_monotonic();

    std::cout << "\nAll StarlingCurve tests passed!\n";
    return 0;
}