#include "Statistics.h"
#include <numeric>
#include <algorithm>

void Statistics::record(int heartRate, int rap, int lap, int pap, int aop, int errorRight, int errorLeft) {
    m_hr.push_back(heartRate);
    m_rap.push_back(rap);
    m_lap.push_back(lap);
    m_pap.push_back(pap);
    m_aop.push_back(aop);
    m_errorRight.push_back(errorRight);
    m_errorLeft.push_back(errorLeft);
}

int Statistics::average(const std::vector<int>& v) {
    if (v.empty()) return 0;
    return std::accumulate(v.begin(), v.end(), 0) / static_cast<int>(v.size());
}

int Statistics::minimum(const std::vector<int>& v) {
    if (v.empty()) return 0;
    return *std::min_element(v.begin(), v.end());
}

int Statistics::maximum(const std::vector<int>& v) {
    if (v.empty()) return 0;
    return *std::max_element(v.begin(), v.end());
}

int Statistics::avgHR() const { return average(m_hr); }
int Statistics::avgRAP() const { return average(m_rap); }
int Statistics::avgLAP() const { return average(m_lap); }
int Statistics::avgPAP() const { return average(m_pap); }
int Statistics::avgAoP() const { return average(m_aop); }
int Statistics::avgErrorRight() const { return average(m_errorRight); }
int Statistics::avgErrorLeft() const { return average(m_errorLeft); }

int Statistics::minRAP() const { return minimum(m_rap); }
int Statistics::maxRAP() const { return maximum(m_rap); }
int Statistics::minLAP() const { return minimum(m_lap); }
int Statistics::maxLAP() const { return maximum(m_lap); }
int Statistics::minPAP() const { return minimum(m_pap); }
int Statistics::maxPAP() const { return maximum(m_pap); }
int Statistics::minAoP() const { return minimum(m_aop); }
int Statistics::maxAoP() const { return maximum(m_aop); }

int Statistics::getCount() const { return static_cast<int>(m_hr.size()); }

void Statistics::clear() {
    m_hr.clear();
    m_rap.clear();
    m_lap.clear();
    m_pap.clear();
    m_aop.clear();
    m_errorRight.clear();
    m_errorLeft.clear();
}