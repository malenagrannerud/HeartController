    };
}

std::vector<std::pair<float, float>> StarlingCurve::getDefaultLVPoints() {
    return {
        {0.0f, 0.0f},
        {5.0f, 70.0f},
        {12.0f, 100.0f},
        {20.0f, 120.0f}
    };
}

StarlingCurve::StarlingCurve(const std::vector<std::pair<float, float>>& points)
    : m_breakpoints(points)
{
    std::sort(m_breakpoints.begin(), m_breakpoints.end());
}

float StarlingCurve::evaluate(float preload) const {
    if (m_breakpoints.empty()) return 0.0f;
    if (preload <= m_breakpoints[0].first) return m_breakpoints[0].second;
    if (preload >= m_breakpoints.back().first) return m_breakpoints.back().second;
    
    for (size_t i = 0; i < m_breakpoints.size() - 1; i++) {
        if (preload >= m_breakpoints[i].first && preload <= m_breakpoints[i+1].first) {
            float t = (preload - m_breakpoints[i].first) / 
                      (m_breakpoints[i+1].first - m_breakpoints[i].first);
            return m_breakpoints[i].second + 
                   t * (m_breakpoints[i+1].second - m_breakpoints[i].second);
        }
    }
    return 0.0f;
}