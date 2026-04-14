/**
 * @file test_circulation.cpp
 * @brief Unit tests for CirculationModel
 * 
 * Compile and run: g++ -std=c++17 test_circulation.cpp -o test_circulation && ./test_circulation
 */

#include <iostream>
#include <cassert>

// Constants from CirculationModel.cpp - JUSTERADE TILL RIMLIGA VÄRDEN
constexpr float DEFAULT_HR = 72.0f;
constexpr float DEFAULT_RAP = 4.0f;
constexpr float DEFAULT_LAP = 9.0f;

constexpr float MIN_RAP = 0.0f;     // Allow 0 for extreme cases
constexpr float MAX_RAP = 30.0f;    // Allow higher for extreme cases

constexpr float MIN_LAP = 0.0f;     // Allow 0 for extreme cases
constexpr float MAX_LAP = 40.0f;    // Allow higher for extreme cases

// JUSTERADE: Rimligare kopplingskoefficienter
constexpr float AOP_EFFECTS_RAP = 0.02f;   
constexpr float PAP_EFFECTS_LAP = 0.03f;   
constexpr float HR_EFFECTS_RAP_OR_LAP = 0.05f;  

float updateRAP(float actualAoP, float HR) {
    float aopEffect = actualAoP * AOP_EFFECTS_RAP;
    float rap = DEFAULT_RAP - aopEffect;
    if (rap < MIN_RAP) rap = MIN_RAP;
    if (rap > MAX_RAP) rap = MAX_RAP;
    rap += (HR - DEFAULT_HR) * HR_EFFECTS_RAP_OR_LAP;
    return rap;
}

float updateLAP(float actualPAP, float HR) {
    float papEffect = actualPAP * PAP_EFFECTS_LAP;
    float lap = DEFAULT_LAP - papEffect;
    if (lap < MIN_LAP) lap = MIN_LAP;
    if (lap > MAX_LAP) lap = MAX_LAP;
    lap += (HR - DEFAULT_HR) * HR_EFFECTS_RAP_OR_LAP;
    return lap;
}

int main() {
    std::cout << "\n========== CIRCULATION MODEL TESTS ==========\n\n";

    /** Test 1: Resting state (HR=72, normal AOP=87, normal PAP=22)



    */
    float rap = updateRAP(87.0f, 72.0f);
    float lap = updateLAP(22.0f, 72.0f);
    std::cout << "Resting: RAP=" << rap << " mmHg (expected ~2.3), LAP=" << lap << " mmHg (expected ~8.3)\n";
    assert(rap > 2.0f && rap < 2.5f);   // 4 - (87*0.02) = 4 - 1.74 = 2.26
    assert(lap > 8.0f && lap < 8.5f);   // 9 - (22*0.03) = 9 - 0.66 = 8.34

    // Test 2: High AoP reduces RAP
    float rap_high = updateRAP(150.0f, 72.0f);
    std::cout << "High AoP (150): RAP=" << rap_high << " mmHg (should be lower than " << rap << ")\n";
    assert(rap_high < rap);

    // Test 3: High PAP reduces LAP
    float lap_high = updateLAP(50.0f, 72.0f);
    std::cout << "High PAP (50): LAP=" << lap_high << " mmHg (should be lower than " << lap << ")\n";
    assert(lap_high < lap);

    // Test 4: High HR increases both
    float rap_fast = updateRAP(87.0f, 120.0f);
    float lap_fast = updateLAP(22.0f, 120.0f);
    std::cout << "High HR (120): RAP=" << rap_fast << ", LAP=" << lap_fast << " (should be higher than resting)\n";
    assert(rap_fast > rap);
    assert(lap_fast > lap);

    // Test 5: Clamping - values stay within limits
    float rap_min = updateRAP(500.0f, 72.0f);
    float rap_max = updateRAP(0.0f, 72.0f);
    float lap_min = updateLAP(500.0f, 72.0f);
    float lap_max = updateLAP(0.0f, 72.0f);
    std::cout << "Clamping: RAP in [" << rap_min << " to " << rap_max 
              << "], LAP in [" << lap_min << " to " << lap_max << "]\n";
    assert(rap_min >= MIN_RAP && rap_min <= MAX_RAP);
    assert(rap_max >= MIN_RAP && rap_max <= MAX_RAP);
    assert(lap_min >= MIN_LAP && lap_min <= MAX_LAP);
    assert(lap_max >= MIN_LAP && lap_max <= MAX_LAP);

    std::cout << "\n========== ALL TESTS PASSED ==========\n\n";
    return 0;
}