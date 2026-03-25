#include "../include/heart_model.h"
#include <cmath>
#include <algorithm>

HeartModel::HeartModel() : preload(5.0), phase(0), heartRate(72) {}

void HeartModel::update(float dt) {
    phase += dt * (heartRate / 60.0) * 2 * M_PI;
    if (phase > 2 * M_PI) phase -= 2 * M_PI;
    float variation = sin(phase) * 0.5;
    preload = preload + variation;
    preload = std::max(0.0f, std::min(20.0f, preload));
}

float HeartModel::getInletPressure() { return preload; }
void HeartModel::setPreload(float p) { preload = std::max(0.0f, std::min(20.0f, p)); }
float HeartModel::getHeartRate() { return heartRate; }