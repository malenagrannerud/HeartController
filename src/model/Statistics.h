/**
 * @file Statistics.h
 * @brief Statistics collection and analysis for TAH simulation
 */

#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>

class Statistics {
public:
    void record(int heartRate, int rap, int lap, int pap, int aop, int errorRight, int errorLeft);
    
    int avgHR() const;
    int avgRAP() const;
    int avgLAP() const;
    int avgPAP() const;
    int avgAoP() const;
    int avgErrorRight() const;
    int avgErrorLeft() const;
    
    int minRAP() const;
    int maxRAP() const;
    int minLAP() const;
    int maxLAP() const;
    int minPAP() const;
    int maxPAP() const;
    int minAoP() const;
    int maxAoP() const;
    
    int getCount() const;
    void clear();
    
private:
    static int average(const std::vector<int>& v);
    static int minimum(const std::vector<int>& v);
    static int maximum(const std::vector<int>& v);
    
    std::vector<int> m_hr;
    std::vector<int> m_rap;
    std::vector<int> m_lap;
    std::vector<int> m_pap;
    std::vector<int> m_aop;
    std::vector<int> m_errorRight;
    std::vector<int> m_errorLeft;
};

#endif