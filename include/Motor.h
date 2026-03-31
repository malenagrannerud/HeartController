/**
 * @file Motor.h
 * @brief Pump motor simulation with first-order dynamics
 */

#ifndef MOTOR_H
#define MOTOR_H

/**
 * @brief Simulates a pump motor with first-order response
 * 
 * Converts target pressure to RPM, then applies first-order dynamics
 * with time constant TAU. Handles RPM to pressure and voltage conversions.
 */
class Motor {
public:
    /**
     * @brief Constructor
     * @param maxPressure Maximum pressure this motor can generate (mmHg)
     */
    explicit Motor(float maxPressure);
    
    /**
     * @brief Initialize motor to a default RPM
     * @param defaultRPM Starting RPM
     */
    void initialize(float defaultRPM);
    
    /**
     * @brief Update motor dynamics
     * @param dt Time step (seconds)
     */
    void update(float dt);
    
    /**
     * @brief Set target pressure for the motor
     * @param targetPressure Desired pressure (mmHg)
     */
    void setTargetPressure(float targetPressure);
    
    /**
     * @brief Reduce motor speed by a factor
     * @param reductionFactor Multiply current speed by this (e.g., 0.8f)
     */
    void reduceSpeed(float reductionFactor);
    
    // ========== Getters ==========
    
    /** @return Actual pressure generated (mmHg) */
    float getActualPressure() const;
    
    /** @return Actual motor speed (RPM) */
    float getActualRPM() const;
    
    /** @return Actual motor voltage (V) */
    float getActualVoltage() const;
    
    /** @return Target motor speed (RPM) */
    float getSetpointRPM() const;
    
    /** @return Target motor voltage (V) */
    float getSetpointVoltage() const;
    
    /** @return Maximum pressure this motor can generate (mmHg) */
    float getMaxPressure() const;
    
private:
    // ========== Conversion functions ==========
    
    /**
     * @brief Convert pressure to required RPM
     * @param pressure Desired pressure (mmHg)
     * @return RPM needed to achieve that pressure
     */
    float pressureToRPM(float pressure) const;
    
    /**
     * @brief Convert RPM to generated pressure
     * @param rpm Motor speed (RPM)
     * @return Pressure generated (mmHg)
     */
    float rpmToPressure(float rpm) const;
    
    /**
     * @brief Convert RPM to motor voltage
     * @param rpm Motor speed (RPM)
     * @return Motor voltage (V)
     */
    float rpmToVoltage(float rpm) const;
    
    // ========== Member variables ==========
    
    float m_maxPressure;    // Maximum pressure (mmHg)
    float m_maxRPM;         // Maximum speed (RPM)
    float m_maxVoltage;     // Maximum voltage (V)
    float m_tau;            // Time constant (seconds)
    
    float m_setpointRPM;    // Target RPM
    float m_currentRPM;     // Actual RPM
    float m_currentVoltage; // Actual voltage (V)
};

#endif