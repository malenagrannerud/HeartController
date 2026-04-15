/**
 * @file test_motor.cpp
 * @brief Unit tests for Motor class
 */

#include "../src/model/Motor.h"
#include <cassert>
#include <iostream>
#include <cmath>

#define TEST(name) void name()
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))
#define ASSERT_TRUE(cond) assert(cond)
#define ASSERT_GT(a, b) assert((a) > (b))

TEST(test_motor_initialization) {
    Motor motor(100.0f);
    motor.initialize(5000.0f);
    
    ASSERT_NEAR(motor.getActualRPM(), 5000.0f, 1.0f);
    ASSERT_TRUE(motor.getActualVoltage() > 0.0f);
    
    std::cout << "✓ Motor initialization correct\n";
}

TEST(test_pressure_to_rpm_conversion) {
    Motor motor(100.0f);
    
    // Zero pressure should give zero RPM
    motor.setTargetPressure(0.0f);
    ASSERT_NEAR(motor.getSetpointRPM(), 0.0f, 1.0f);
    
    // Max pressure should give max RPM
    motor.setTargetPressure(100.0f);
    ASSERT_NEAR(motor.getSetpointRPM(), Motor::MAX_RPM, 1.0f);
    
    std::cout << "✓ Pressure to RPM conversion correct\n";
}

TEST(test_motor_dynamics) {
    Motor motor(100.0f);
    motor.initialize(0.0f);
    
    // Set target and update
    motor.setTargetPressure(50.0f);
    float targetRPM = motor.getSetpointRPM();
    
    // Update a few times
    for (int i = 0; i < 10; ++i) {
        motor.update(0.05f);
    }
    
    float actualRPM = motor.getActualRPM();
    ASSERT_GT(actualRPM, 0.0f);
    ASSERT_TRUE(actualRPM <= targetRPM);
    
    std::cout << "✓ Motor dynamics working\n";
}

TEST(test_reduce_speed) {
    Motor motor(100.0f);
    motor.initialize(5000.0f);
    
    float beforeRPM = motor.getActualRPM();
    motor.reduceSpeed(0.5f);
    float afterRPM = motor.getActualRPM();
    
    ASSERT_NEAR(afterRPM, beforeRPM * 0.5f, 1.0f);
    
    std::cout << "✓ Reduce speed works\n";
}

TEST(test_voltage_limits) {
    Motor motor(100.0f);
    motor.initialize(Motor::MAX_RPM);
    
    float voltage = motor.getActualVoltage();
    ASSERT_TRUE(voltage <= Motor::MAX_VOLTAGE);
    ASSERT_TRUE(voltage >= 0.0f);
    
    std::cout << "✓ Voltage within limits\n";
}

int main() {
    std::cout << "Running Motor tests...\n\n";
    
    test_motor_initialization();
    test_pressure_to_rpm_conversion();
    test_motor_dynamics();
    test_reduce_speed();
    test_voltage_limits();
    
    std::cout << "\n✅ All Motor tests passed!\n";
    return 0;
}