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

    // testpunkter: {0.0, 0.0}, {2.0, 4.0}, {4.0, 5.0}, {7.0, 6.5}, {12.0, 8.0}
    ASSERT_NEAR(rv.evaluate(0.0f, 72.0f), 0.0f, 0.1f);   // Punkt 1
    ASSERT_NEAR(rv.evaluate(2.0f, 72.0f), 4.0f, 0.1f);   // Punkt 2 
    ASSERT_NEAR(rv.evaluate(4.0f, 72.0f), 5.0f, 0.1f);   // Punkt 3
    
    // För 6.0 mmHg (mellan punkt 3 och 4):
    // Mellan {4, 5.0} och {7, 6.5} -> vid 6.0 bör det vara 6.0 (linjärt)
    ASSERT_NEAR(rv.evaluate(6.0f, 72.0f), 6.0f, 0.1f);   
    ASSERT_NEAR(rv.evaluate(12.0f, 72.0f), 8.0f, 0.1f);  // Punkt 5 
    std::cout << "✅ RV curve values correct (L/min scales)\n";
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
 * Test interpolation (value between two points).
 */
TEST(test_interpolation) {
    StarlingCurve rv(StarlingCurve::getRVPoints());
    // Midpoint between 2.0 (4.0L) and 4.0 (5.0L) is 3.0 (4.5L)
    ASSERT_NEAR(rv.evaluate(3.0f, 72.0f), 4.5f, 0.01f);
    std::cout << "✅ Interpolation logic correct\n";
}




/**
 * Ensure curves never decrease.
 */
TEST(test_monotonic) {
    StarlingCurve rv(StarlingCurve::getRVPoints());
    
    float prev = -1.0f;
    for (float p = 0.0f; p <= 25.0f; p += 0.5f) {
        float current = rv.evaluate(p, 72.0f);
        ASSERT_TRUE(current >= prev);
        prev = current;
    }
    std::cout << "✅ Curves are monotonically increasing\n";
}


int main() {
    std::cout << "Running StarlingCurve tests...\n\n";
   
    try {
        test_curves_not_empty();
        test_rv_curve();
        test_rv_edge_cases();
        test_lv_curve();
        test_interpolation();
        test_monotonic();
    } catch (...) {
        std::cerr << "\n🔥 Critical error during testing!\n";
        return 1;
    }


    std::cout << "\nAll StarlingCurve tests passed!\n";
    return 0;
}