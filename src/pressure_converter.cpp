
#include "../include/pressure_converter.h"

double PressureConverter::pressureToVoltage(double pressure, ErrorCode& error) {
    if (pressure < 0.0 || pressure > 40.0) {
        error = ErrorCode::PC_PRESSURE_CLIPPED;
        pressure = pressure < 0.0 ? 0.0 : 40.0;
    } else {
        error = ErrorCode::FS_SUCCESS;
    }
    return ZERO_VOLTAGE + pressure * SENSITIVITY;
}

double PressureConverter::voltageToPressure(double voltage, ErrorCode& error) {
    if (voltage < ZERO_VOLTAGE || voltage > MAX_VOLTAGE) {
        error = ErrorCode::PC_VOLTAGE_SATURATION;
        voltage = voltage < ZERO_VOLTAGE ? ZERO_VOLTAGE : MAX_VOLTAGE;
    } else {
        error = ErrorCode::FS_SUCCESS;
    }
    return (voltage - ZERO_VOLTAGE) / SENSITIVITY;
}
