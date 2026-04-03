#include "StarlingCurve.h"
#include <algorithm>

std::vector<std::pair<float, float>> StarlingCurve::getDefaultRVPoints() {
    return {{0,0}, {2,12}, {6,22}, {12,35}};
}

std::vector<std::pair<float, float>> StarlingCurve::getDefaultLVPoints() {
    return {{0,0}, {5,70}, {12,100}, {20,120}};
}

StarlingCurve::StarlingCurve(const std::vector<std::pair<float, float>>& points) : m_breakpoints(points) {
    std::sort(m_breakpoints.begin(), m_breakpoints.end());
}

float StarlingCurve::evaluate(float preload) const {
    if (m_breakpoints.empty()) return 0;
    if (preload <= m_breakpoints[0].first) return m_breakpoints[0].second;
    if (preload >= m_breakpoints.back().first) return m_breakpoints.back().second;
    
    for (size_t i = 0; i < m_breakpoints.size() - 1; i++) {
        if (preload >= m_breakpoints[i].first && preload <= m_breakpoints[i+1].first) {
            float t = (preload - m_breakpoints[i].first) / (m_breakpoints[i+1].first - m_breakpoints[i].first);
            return m_breakpoints[i].second + t * (m_breakpoints[i+1].second - m_breakpoints[i].second);
        }
    }
    return 0;
}
