#include "../include/pressure_converter.h"
#include <algorithm>

double PressureConverter::pressureToVoltage(double pressure) {
    pressure = std::max(0.0, std::min(40.0, pressure));
    return ZERO_VOLTAGE + pressure * SENSITIVITY;
}

double PressureConverter::voltageToPressure(double voltage) {
    voltage = std::max(ZERO_VOLTAGE, std::min(MAX_VOLTAGE, voltage));
    return (voltage - ZERO_VOLTAGE) / SENSITIVITY;
}