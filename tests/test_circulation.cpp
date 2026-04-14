/**
 * @file test_circulation.cpp
 * @brief Unit tests for CirculationModel
 * 
 * Compile and run: g++ -std=c++17 test_circulation.cpp -o test_circulation && ./test_circulation
 */

#include <iostream>
#include <cassert>

// Constants from CirculationModel.cpp
constexpr float DEFAULT_HR = 72.0f;
constexpr float DEFAULT_RAP = 4.0f;
constexpr float DEFAULT_LAP = 9.0f;

constexpr float MIN_RAP = 0.0f;     // <2 is patological, but we allow it to go to 0 for testing and extreme cases
constexpr float MAX_RAP = 30.0f;    // >15 is patological, but we allow it to go higher for testing and extreme cases

constexpr float MIN_LAP = 0.0f;     // <6 patological, but allowed here 0 for testing and extreme cases
constexpr float MAX_LAP = 40.0f;    // >25 patological, but allowed here 0 for testing and extreme cases

constexpr float AOP_EFFECT_ON_RAP = 0.05f;
constexpr float PAP_EFFECT_ON_LAP = 0.08f;
constexpr float HR_EFFECT_ON_VENOUS_RETURN = 0.05f;



float calculateNewRAP(float actualAoP, float heartRate) {
    float aopEffect = actualAoP * AOP_EFFECT_ON_RAP;
    float rap = DEFAULT_RAP - aopEffect;
    if (rap < MIN_RAP) rap = MIN_RAP;
    if (rap > MAX_RAP) rap = MAX_RAP;
    rap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    return rap;
}

float calculateNewLAP(float actualPAP, float heartRate) {
    float papEffect = actualPAP * PAP_EFFECT_ON_LAP;
    float lap = DEFAULT_LAP - papEffect;
    if (lap < MIN_LAP) lap = MIN_LAP;
    if (lap > MAX_LAP) lap = MAX_LAP;
    lap += (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN;
    return lap;
}

int main() {
    std::cout << "\n========== CIRCULATION MODEL TESTS ==========\n\n";

    // Test 1: Resting state (HR=72, normal AOP=87, normal PAP=22)
    float rap = calculateNewRAP(87.0f, 72.0f);
    float lap = calculateNewLAP(22.0f, 72.0f);
    std::cout << "Resting: RAP=" << rap << " mmHg (expected ~4), LAP=" << lap << " mmHg (expected ~9)\n";
    assert(rap > 3.5f && rap < 4.5f);
    assert(lap > 8.5f && lap < 9.5f);

    // Test 2: High AoP reduces RAP
    float rap_high = calculateNewRAP(150.0f, 72.0f);
    std::cout << "High AoP (150): RAP=" << rap_high << " mmHg (should be lower than " << rap << ")\n";
    assert(rap_high < rap);

    // Test 3: High PAP reduces LAP
    float lap_high = calculateNewLAP(50.0f, 72.0f);
    std::cout << "High PAP (50): LAP=" << lap_high << " mmHg (should be lower than " << lap << ")\n";
    assert(lap_high < lap);

    // Test 4: High HR increases both
    float rap_fast = calculateNewRAP(87.0f, 120.0f);
    float lap_fast = calculateNewLAP(22.0f, 120.0f);
    std::cout << "High HR (120): RAP=" << rap_fast << ", LAP=" << lap_fast << " (should be higher than resting)\n";
    assert(rap_fast > rap);
    assert(lap_fast > lap);

    // Test 5: Clamping - values stay within limits
    float rap_min = calculateNewRAP(500.0f, 72.0f);
    float rap_max = calculateNewRAP(0.0f, 72.0f);
    float lap_min = calculateNewLAP(500.0f, 72.0f);
    float lap_max = calculateNewLAP(0.0f, 72.0f);
    std::cout << "Clamping: RAP in [" << rap_min << " to " << rap_max 
              << "], LAP in [" << lap_min << " to " << lap_max << "]\n";
    assert(rap_min >= MIN_RAP && rap_min <= MAX_RAP);
    assert(rap_max >= MIN_RAP && rap_max <= MAX_RAP);
    assert(lap_min >= MIN_LAP && lap_min <= MAX_LAP);
    assert(lap_max >= MIN_LAP && lap_max <= MAX_LAP);

    std::cout << "\n========== ALL TESTS PASSED ==========\n\n";
    return 0;
}