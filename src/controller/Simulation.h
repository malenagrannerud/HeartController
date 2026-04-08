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
    
    int m_heartRate;
    int m_simTime;
    bool m_running;
    int m_rap;
    int m_lap;
    
    PressureSensor m_rapSensor;
    PressureSensor m_lapSensor;
    Motor m_rightPump;
    Motor m_leftPump;
    StarlingCurve m_starlingRV;
    StarlingCurve m_starlingLV;
    
    struct Stats {
        std::vector<int> hr, rap, lap, pap, aop, errR, errL;
        void add(int h, int r, int la, int p, int a, int er, int el);
        int avg(const std::vector<int>& v);
        int min(const std::vector<int>& v);
        int max(const std::vector<int>& v);
    } m_stats;
};

#endif