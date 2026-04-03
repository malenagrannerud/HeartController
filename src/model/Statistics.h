
#include "Statistics.h"
#include <numeric>
#include <algorithm>

void Statistics::add(float _hr, float _rap, float _lap, float _pap, float _aop, 
                      float _errR, float _errL) {
    hr.push_back(_hr);
    rap.push_back(_rap);
    lap.push_back(_lap);
    pap.push_back(_pap);
    aop.push_back(_aop);
    errorRight.push_back(_errR);
    errorLeft.push_back(_errL);
}

float Statistics::avg(const std::vector<float>& v) {
    if (v.empty()) return 0;
    return std::accumulate(v.begin(), v.end(), 0.0f) / v.size();
}

float Statistics::minVal(const std::vector<float>& v) {
    if (v.empty()) return 0;
    return *std::min_element(v.begin(), v.end());
}

float Statistics::maxVal(const std::vector<float>& v) {
    if (v.empty()) return 0;
    return *std::max_element(v.begin(), v.end());
}