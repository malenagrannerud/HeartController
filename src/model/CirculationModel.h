/**
 * @file CirculationModel.h
 * @brief Body's physiological response - pressure is result of flow
 */

#ifndef CIRCULATION_MODEL_H
#define CIRCULATION_MODEL_H

class CirculationModel {
public:
    CirculationModel();
    void update(float hr, float coRV, float coLV, float dt);
    void reset();
    
    float getRAP() const;
    float getLAP() const;
    float getPAP() const;
    float getAoP() const;
    float getCO_RV() const;
    float getCO_LV() const;
    float getBalance() const;
    
private:
    // Intitial values 
    static constexpr float DEFAULT_HR = 72.0f;
    static constexpr float DEFAULT_RAP = 4.0f;
    static constexpr float DEFAULT_LAP = 8.0f;
    static constexpr float DEFAULT_PAP = 20.0f;
    static constexpr float DEFAULT_AOP = 90.0f;
    
    // --- Safety Limits ---
    // Increase MIN to prevent suction alarms too early; Decrease MAX to cap how high pressure can go.
    static constexpr float MIN_RAP = 1.0f;
    static constexpr float MAX_RAP = 25.0f;
    static constexpr float MIN_LAP = 1.0f;
    static constexpr float MAX_LAP = 35.0f;
    static constexpr float MAX_CO = 12.0f; 

    
    // --- Vascular Resistance ---
    // Higher values = Higher actual pressures (AoP/PAP) for the same CO.
    static constexpr float SYSTEMIC_RESISTANCE = 1200.0f;  // Recistance of body, affects AOP
    static constexpr float PULMONARY_RESISTANCE = 250.0f;  // Recistance of lungs, affects PAP
    

    // --- Venous Return (Feedback) ---
    // These act as the "faucet" for blood returning to the heart.
    // INCREASE these if RAP/LAP stay too low; DECREASE these if RAP/LAP "run away" to MAX values.

    static constexpr float AOP_EFFECT_ON_RAP = 0.001f;
    static constexpr float PAP_EFFECT_ON_LAP = 0.001f;
    
    float m_hr;
    float m_rap;
    float m_lap;
    float m_pap;
    float m_aop;
    float m_coRV;
    float m_coLV;
    
    float clamp(float value, float min, float max) const;
};

#endif