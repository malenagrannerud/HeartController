#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

class PressureSensor {
public:
    PressureSensor();
    float measure(float truePressure);
};

#endif