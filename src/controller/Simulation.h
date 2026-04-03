#ifndef SIMULATION_H
#define SIMULATION_H

#include "model/PressureSensor.h"
#include "model/Motor.h"
#include "model/StarlingCurve.h"
#include "model/CirculationModel.h"
#include "view/Terminal.h"
#include <vector>

class Simulation {
public:
    Simulation();
    void run();
    
private:
    void handleUserInput();
    
    float m_heartRate;
    float m_simTime;
    bool m_running;
    float m_rap;
    float m_lap;
    
    PressureSensor m_rapSensor;
    PressureSensor m_lapSensor;
    Motor m_rightPump;
    Motor m_leftPump;
    StarlingCurve m_starlingRV;
    StarlingCurve m_starlingLV;
    
    struct Stats {
        std::vector<float> hr, rap, lap, pap, aop, errR, errL;
        void add(float h, float r, float la, float p, float a, float er, float el);
        float avg(const std::vector<float>& v);
        float min(const std::vector<float>& v);
        float max(const std::vector<float>& v);
    } m_stats;
};

#endif