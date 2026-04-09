/**
 * @file Motor.h
 * @brief Models one motor for one ventricle
 */

#ifndef MOTOR_H
#define MOTOR_H

/**
 * @class Motor
 * @brief Model of centrifugal pump motor with first-order dynamics
 * 
 * Simulates motor response with:
 * - Pressure ∝ RPM² (affinity laws)
 * - First-order lag (time constant TAU)
 * - Voltage control
 */

class Motor {
public:
    static constexpr int MAX_RPM = 10000;   // Must produce at least CO=10 L/min
    static constexpr int MAX_VOLTAGE = 20;  // Bivacore≈24V, Carmat≈20V, Realheart≈20V
    static constexpr float TAU = 0.25f;
    
    explicit Motor(float maxPressure);
    void initialize(float defaultRPM);
    void update(float dt);
    void setTargetPressure(float targetPressure);
    void reduceSpeed(float reductionFactor);
    float getActualPressure() const;
    float getActualRPM() const;
    float getActualVoltage() const;
    float getSetpointRPM() const;
    float getSetpointVoltage() const;
    float getMaxPressure() const;
    
private:
    float pressureToRPM(float pressure) const;
    float rpmToPressure(float rpm) const;
    float rpmToVoltage(float rpm) const;
    
    float m_maxPressure;
    float m_setpointRPM;
    float m_currentRPM;
    float m_currentVoltage;
};

#endif