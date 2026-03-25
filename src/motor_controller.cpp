#include "../include/motor_controller.h"
#include <algorithm>

MotorController::MotorController() : targetPressure(0), currentPressure(0), currentRPM(0), maxRPM(4720) {}

void MotorController::setTargetPressure(float p) {
    targetPressure = std::max(0.0f, std::min(40.0f, p));
}

void MotorController::update(float dt) {
    float diff = targetPressure - currentPressure;
    currentPressure += diff * dt * 10;
    currentRPM = (currentPressure / 40.0f) * maxRPM;
}

float MotorController::getCurrentPressure() { return currentPressure; }
float MotorController::getCurrentRPM() { return currentRPM; }
float MotorController::getCurrentDraw() { 
    float load = currentPressure / 40.0f;
    return 500 + load * load * 2000;
}