#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

class PressureSensor {
public:
    explicit PressureSensor(float maxPressure);
    
    float measure(float truePressure);
    float pressureToVoltage(float pressure) const;
    
private:
    float m_maxPressure;
};

#endif 