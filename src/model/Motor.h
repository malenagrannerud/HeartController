/**
 * @file Motor.h
 * @brief Pump motor - flow controlled
 */

#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:
    static constexpr float MAX_RPM = 10000.0f;
    static constexpr float MAX_VOLTAGE = 20.0f;
    static constexpr float TAU = 0.25f;
    
    explicit Motor(float maxFlow);
    void initialize(float startRPM);
    void update(float dt);
    
    void setTargetFlow(float flow);
    void reduceSpeed(float reductionFactor);
    
    float getActualFlow() const;
    float getActualRPM() const;
    float getActualVoltage() const;
    float getSetpointRPM() const;
    
private:
    float flowToRPM(float flow) const;
    float rpmToFlow(float rpm) const;
    float rpmToVoltage(float rpm) const;
    
    float m_maxFlow;
    float m_setpointRPM;
    float m_currentRPM;
    float m_currentVoltage;
};

#endif