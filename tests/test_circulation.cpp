/**
 * @file test_circulation.cpp
 * @brief Unit tests for CirculationModel class
 */

#include "../src/model/CirculationModel.h"
#include <cassert>
#include <iostream>
#include <cmath>

#define TEST(name) void name()
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))
#define ASSERT_TRUE(cond) assert(cond)
#define ASSERT_GT(a, b) assert((a) > (b))
#define ASSERT_LT(a, b) assert((a) < (b))

TEST(test_default_initialization) {
    CirculationModel model;
    
    // Default values should be physiological
    ASSERT_NEAR(model.getHR(), 72.0f, 0.1f);
    ASSERT_NEAR(model.getRAP(), 6.0f, 1.0f);
    ASSERT_NEAR(model.getLAP(), 10.0f, 1.0f);
    ASSERT_NEAR(model.getAoP(), 90.0f, 10.0f);
    ASSERT_NEAR(model.getPAP(), 15.0f, 5.0f);
    ASSERT_NEAR(model.getCO(), 5.0f, 1.0f);
    
    std::cout << "✓ Default initialization correct\n";
}

TEST(test_reset_function) {
    CirculationModel model;
    
    // Change state
    model.update(120.0f);
    
    // Reset
    model.reset();
    
    // Should be back to defaults
    ASSERT_NEAR(model.getHR(), 72.0f, 0.1f);
    ASSERT_NEAR(model.getRAP(), 6.0f, 1.0f);
    ASSERT_NEAR(model.getLAP(), 10.0f, 1.0f);
    
    std::cout << "✓ Reset function works\n";
}

TEST(test_hr_increases_co) {
    CirculationModel model;
    model.reset();
    
    float co_baseline = model.getCO();
    
    // Increase HR
    model.update(120.0f);
    float co_increased = model.getCO();
    
    ASSERT_GT(co_increased, co_baseline);
    
    std::cout << "✓ Higher HR increases cardiac output\n";
}

TEST(test_series_circulation_balance) {
    CirculationModel model;
    model.reset();
    
    // After several updates, RV and LV should balance
    for (int i = 0; i < 10; ++i) {
        model.update(72.0f);
    }
    
    float svRV = model.getRVStrokeVolume();
    float svLV = model.getLVStrokeVolume();
    float sv = model.getSV();
    
    // Effective SV should be min of RV and LV
    ASSERT_NEAR(sv, std::min(svRV, svLV), 0.1f);
    
    std::cout << "✓ Series circulation balance maintained\n";
}

TEST(test_preload_limits) {
    CirculationModel model;
    
    // Run many updates to see if preload stays within limits
    for (int i = 0; i < 100; ++i) {
        model.update(72.0f);
        float rap = model.getRAP();
        float lap = model.getLAP();
        
        ASSERT_TRUE(rap >= 1.0f && rap <= 25.0f);
        ASSERT_TRUE(lap >= 2.0f && lap <= 35.0f);
    }
    
    std::cout << "✓ Preload stays within physiological limits\n";
}

TEST(test_hr_clamping) {
    CirculationModel model;
    
    // Test below minimum
    model.update(20.0f);
    ASSERT_NEAR(model.getHR(), 40.0f, 0.1f);
    
    // Test above maximum
    model.update(200.0f);
    ASSERT_NEAR(model.getHR(), 180.0f, 0.1f);
    
    std::cout << "✓ HR clamped to valid range\n";
}

TEST(test_steady_state) {
    CirculationModel model;
    model.reset();
    
    float prev_co = model.getCO();
    bool reached_steady_state = false;
    
    // Run until steady state or max iterations
    for (int i = 0; i < 50; ++i) {
        model.update(72.0f);
        float current_co = model.getCO();
        
        if (std::abs(current_co - prev_co) < 0.01f) {
            reached_steady_state = true;
            break;
        }
        prev_co = current_co;
    }
    
    ASSERT_TRUE(reached_steady_state);
    
    std::cout << "✓ System reaches steady state\n";
}

TEST(test_pressure_flow_relationship) {
    CirculationModel model;
    model.reset();
    
    model.update(72.0f);
    
    float co = model.getCO();
    float aop = model.getAoP();
    float pap = model.getPAP();
    
    // Pressure should be proportional to flow (approximately)
    // AoP/CO should be roughly constant (systemic resistance)
    float systemic_ratio = aop / co;
    float pulmonary_ratio = pap / co;
    
    ASSERT_TRUE(systemic_ratio > 10.0f && systemic_ratio < 20.0f);
    ASSERT_TRUE(pulmonary_ratio > 1.0f && pulmonary_ratio < 5.0f);
    
    std::cout << "✓ Pressure-flow relationship correct\n";
}

int main() {
    std::cout << "Running CirculationModel tests...\n\n";
    
    test_default_initialization();
    test_reset_function();
    test_hr_increases_co();
    test_series_circulation_balance();
    test_preload_limits();
    test_hr_clamping();
    test_steady_state();
    test_pressure_flow_relationship();
    
    std::cout << "\n✅ All CirculationModel tests passed!\n";
    return 0;
}