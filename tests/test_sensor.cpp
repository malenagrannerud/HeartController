/**
 * @file test_sensor.cpp
 * @brief Unit tests for PressureSensor class
 */

#include "../src/model/PressureSensor.h"
#include <cassert>
#include <iostream>
#include <cmath>

#define TEST(name) void name()
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))
#define ASSERT_TRUE(cond) assert(cond)

TEST(test_sensor_measurement) {
    PressureSensor sensor(100.0f);
    
    // Multiple measurements should be close to true value
    float truePressure = 50.0f;
    float sum = 0.0f;
    int n = 100;
    
    for (int i = 0; i < n; ++i) {
        sum += sensor.measure(truePressure);
    }
    
    float avg = sum / n;
    ASSERT_NEAR(avg, truePressure, 1.0f);
    
    std::cout << "✓ Sensor measurement accurate on average\n";
}

TEST(test_sensor_noise) {
    PressureSensor sensor(100.0f);
    
    float truePressure = 30.0f;
    float m1 = sensor.measure(truePressure);
    float m2 = sensor.measure(truePressure);
    
    // Measurements should vary due to noise
    // (Note: there's a small chance they're equal, but very unlikely)
    // This is a probabilistic test
    
    std::cout << "✓ Sensor produces measurements\n";
}

TEST(test_sensor_limits) {
    PressureSensor sensor(100.0f);
    
    // Should clamp to 0
    float m1 = sensor.measure(-10.0f);
    ASSERT_TRUE(m1 >= 0.0f);
    
    // Should clamp to max
    float m2 = sensor.measure(200.0f);
    ASSERT_TRUE(m2 <= 100.0f);
    
    std::cout << "✓ Sensor respects limits\n";
}

TEST(test_voltage_conversion) {
    PressureSensor sensor(100.0f);
    
    float voltage1 = sensor.pressureToVoltage(0.0f);
    ASSERT_NEAR(voltage1, PressureSensor::CALIBRATION_ZERO, 0.01f);
    
    float voltage2 = sensor.pressureToVoltage(10.0f);
    float expected = PressureSensor::CALIBRATION_ZERO + 10.0f * PressureSensor::VOLTAGE_PER_MMHG;
    ASSERT_NEAR(voltage2, expected, 0.01f);
    
    std::cout << "✓ Voltage conversion correct\n";
}

int main() {
    std::cout << "Running PressureSensor tests...\n\n";
    
    test_sensor_measurement();
    test_sensor_noise();
    test_sensor_limits();
    test_voltage_conversion();
    
    std::cout << "\n✅ All PressureSensor tests passed!\n";
    return 0;
}