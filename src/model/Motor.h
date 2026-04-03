#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:
    static constexpr float MAX_RPM = 4720.0f;
    static constexpr float MAX_VOLTAGE = 12.0f;
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
