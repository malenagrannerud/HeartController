#ifndef PRESSURE_CONVERTER_H
#define PRESSURE_CONVERTER_H

#include "error_codes.h"

class PressureConverter {
public:
    static constexpr double SENSITIVITY = 0.0097;
    static constexpr double ZERO_VOLTAGE = 0.093;
    static constexpr double MAX_VOLTAGE = 0.6815;
    
    static double pressureToVoltage(double pressure, ErrorCode& error);
    static double voltageToPressure(double voltage, ErrorCode& error);
};

#endif
