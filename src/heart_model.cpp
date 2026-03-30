#include "../include/heart_model.h"
#include <cmath>
#include <algorithm>

HeartModel::HeartModel() {
    preload = 3.0f;
    afterload = 15.0f;
    contractility = 1.0f;
    heartRate = 72.0f;
}

void HeartModel::update(float dt) {
    // Preload påverkas av puls (fysiologisk koppling)
    // Ju högre puls, desto högre preload (ökat venöst återflöde)
    float targetPreload = 3.0f + (heartRate - 60.0f) * 0.1f;
    
    // Glidande medelvärde för att undvika hopp
    preload = preload * 0.95f + targetPreload * 0.05f;
    preload = std::max(0.0f, std::min(20.0f, preload));
}

float HeartModel::getInletPressure() {
    return preload;
}

float HeartModel::getOutletPressure() {
    return preload * contractility * 2.5f;
}

float HeartModel::getFlow() {
    float pressureGradient = std::max(0.0f, getOutletPressure() - afterload);
    return pressureGradient * 0.5f;
}

void HeartModel::setPreload(float p) {
    preload = std::max(0.0f, std::min(20.0f, p));
}

void HeartModel::setAfterload(float a) {
    afterload = std::max(5.0f, std::min(30.0f, a));
}

void HeartModel::setHeartRate(float hr) {
    heartRate = std::max(40.0f, std::min(180.0f, hr));
}

float HeartModel::getHeartRate() {
    return heartRate;
}