/**
 * @file Motor.h
 * @brief Models one motor for one ventricle




# ========== RPM P-CONTROLL ==========
# RPMtarget = RPMbase + (RPMmax - RPMbase) * K * (Pin - Psetpoint)



# ========== HANDLE 1:  LEFT RIGHT BALANCE ==========
# LAP = RAP + 5   
# EX: RAP=5mmHg → LAP=5+5=10mmHg 
# EX: RAP=15mmHg → LAP=15+5=20mmHg 

# Logic: 
# rest-> exercise: If sensor on RAP is >10 mmHg & stays there for >10 sek
# exercise-> rest: If sensor on RAP is <10 mmHg and LAP>12 mmHg & stays there for >10 sek
# Emergency: If sensor on RAP is <2 mmHg --> higher RPM immediately. 


# ========== HANDLE 2:  SUCTION PROTECTION ==========


 */

#ifndef MOTOR_H
#define MOTOR_H

/**
 * @class Motor
 * @brief Model of centrifugal pump motor with first-order dynamics
 * 
 * Simulates motor response with:
 * - Pressure ∝ RPM² (affinity laws)    ????????????????????????????
 * - First-order lag (time constant TAU)
 * - Voltage control
 */


class Motor {
public:
    static constexpr int MAX_RPM = 10000;   // Must produce at least CO=10 L/min
    static constexpr int MAX_VOLTAGE = 20; // Bivacore≈24V, Carmat≈20V, Realheart≈20V
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