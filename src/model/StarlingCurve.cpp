#include "StarlingCurve.h"
#include <algorithm>

std::vector<std::pair<float, float>> StarlingCurve::getRVPoints() {
    return {
        {0.0f, 0.0f},
        {2.0f, 4.0f},    // Öka från 3.0
        {4.0f, 5.0f},    // Behåll 5.0
        {7.0f, 6.5f},    // Öka från 6.5
        {12.0f, 8.0f}    // Behåll 8.0
    };
}

std::vector<std::pair<float, float>> StarlingCurve::getLVPoints() {
    return {
        {0.0f, 0.0f},
        {5.0f, 3.5f},
        {8.0f, 5.0f},
        {14.0f, 6.5f},
        {22.0f, 8.0f}
    };
}

StarlingCurve::StarlingCurve(const std::vector<std::pair<float, float>>& points) 
    : m_points(points) {
    std::sort(m_points.begin(), m_points.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
}

float StarlingCurve::evaluate(float preload, float hr) const {
    if (m_points.empty()) return 0.0f;
    
    float coAt72 = 0.0f;
    if (preload <= m_points[0].first) {
        coAt72 = m_points[0].second;
    } else if (preload >= m_points.back().first) {
        coAt72 = m_points.back().second;
    } else {
        for (size_t i = 0; i < m_points.size() - 1; ++i) {
            if (preload >= m_points[i].first && preload <= m_points[i + 1].first) {
                float t = (preload - m_points[i].first) / (m_points[i + 1].first - m_points[i].first);
                coAt72 = m_points[i].second + t * (m_points[i + 1].second - m_points[i].second);
                break;
            }
        }
    }
    return coAt72 * (hr / 72.0f);
}
