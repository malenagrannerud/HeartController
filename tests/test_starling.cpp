/**
 * @file test_starling.cpp
 * @brief Unit tests for StarlingCurve.cpp
 * 
 */

#include "../src/model/StarlingCurve.h" // Tell compiler where to find StarlingCurve class definition, else compilarion error 
#include <cassert>      // Has assert(condition), crashes the program if condition is false
#include <iostream>     // iostream prints to console (std::cout)
#include <cmath>        // Has matematical functions like std::abs

#define TEST(name) void name() // Macro for naming tests, better than void test_default_curves_not_empty() every time. precompiler replaces TEST(test_default_curves_not_empty) with void test_default_curves_not_empty() before compilation.
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))
#define ASSERT_TRUE(cond) assert(cond)



/**
 * @test test_default_curves_not_empty
 * 
 * Creates RV & LV Starling-curves from default values. Tests that they 
 * return a value > 0 at preload = 5 mmHg.
 * Ensures that the curves are not empty or broken. 
 */
TEST(test_default_curves_not_empty) {                       // Declair a test and name it "test_default_curves_not_empty"
    StarlingCurve rv(StarlingCurve::getDefaultRVPoints());  // Create a StarlingCurve-object for RV
    StarlingCurve lv(StarlingCurve::getDefaultLVPoints());
    ASSERT_TRUE(rv.evaluate(5.0f) > 0.0f);
    ASSERT_TRUE(lv.evaluate(5.0f) > 0.0f);
    std::cout << "✅ Default curves not empty\n";
}


/**
 * @test test_rv_curve_values
 * 
 * Tests values of the RV Starling curve at specific preloads.
 */

TEST(test_rv_curve_values) {
    StarlingCurve rv(StarlingCurve::getDefaultRVPoints());
    ASSERT_NEAR(rv.evaluate(0.0f), 0.0f, 0.1f);
    ASSERT_NEAR(rv.evaluate(2.0f), 40.0f, 0.1f);
    ASSERT_NEAR(rv.evaluate(6.0f), 60.0f, 0.1f);
    ASSERT_NEAR(rv.evaluate(12.0f), 70.0f, 0.1f);
    std::cout << "✓ RV curve values correct\n";
}

TEST(test_lv_curve_values) {
    StarlingCurve lv(StarlingCurve::getDefaultLVPoints());
    ASSERT_NEAR(lv.evaluate(0.0f), 0.0f, 0.1f);
    ASSERT_NEAR(lv.evaluate(5.0f), 50.0f, 0.1f);
    ASSERT_NEAR(lv.evaluate(12.0f), 75.0f, 0.1f);
    ASSERT_NEAR(lv.evaluate(20.0f), 90.0f, 0.1f);
    std::cout << "✓ LV curve values correct\n";
}

int main() {
    std::cout << "Running StarlingCurve tests...\n\n";
    test_default_curves_not_empty();
    test_rv_curve_values();
    test_lv_curve_values();
    std::cout << "\n✅ All StarlingCurve tests passed!\n";
    return 0;
}