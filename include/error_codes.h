
/**
 * @file error_codes.h
 * @brief ISO 62304: Definerar felkoder för spårbarhet
 * @version 1.0.0
 * 
 * Enligt ISO 62304 Section 5.2: Riskhantering kräver spårbara felkoder
 */

#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#include <cstdint>

/**
 * @enum ErrorCode
 * @brief Systemfelkoder för spårbarhet och riskhantering
 * 
 * Kodstruktur: [Modul][Allvarlighet][Unikt ID]
 * - Modul: 1-9 (1:FS, 2:CV, 3:PC, 4:CA, 5:HM, 6:PS, 7:MC, 8:DI, 9:MA)
 * - Allvarlighet: 0=Fatal, 1=Kritisk, 2=Varning, 3=Info
 */
enum class ErrorCode : uint32_t {
    // Frank-Starling (FS) - Modul 1
    FS_SUCCESS = 0x01000000,
    FS_INVALID_PRELOAD = 0x01100001,      // Kritisk: Ogiltigt inloppstryck
    FS_SATURATION_WARNING = 0x01200002,   // Varning: Mättnad uppnådd
    FS_CURVE_MONOTONIC = 0x01300003,      // Info: Kurva är monoton
    
    // Curve Validator (CV) - Modul 2
    CV_INVALID_POINTS = 0x02100001,       // Kritisk: Otillräckligt antal punkter
    CV_NON_MONOTONIC = 0x02100002,        // Kritisk: Kurva är inte monoton
    CV_OUT_OF_RANGE = 0x02100003,         // Kritisk: Värde utanför tillåtet område
    
    // Pressure Converter (PC) - Modul 3
    PC_VOLTAGE_SATURATION = 0x02200001,   // Varning: Spänning mättad
    PC_PRESSURE_CLIPPED = 0x02200002,     // Varning: Tryck klippt
    
    // Curve Analyzer (CA) - Modul 4
    CA_GAIN_ZERO = 0x02300001,            // Info: Förstärkning noll
    CA_AREA_CALCULATED = 0x03300002,      // Info: Area beräknad
    
    // Heart Model (HM) - Modul 5
    HM_INVALID_HEART_RATE = 0x01100001,   // Kritisk: Ogiltig puls
    HM_PRELOAD_LIMIT = 0x01200002,        // Varning: Preload vid gräns
    
    // Pressure Sensor (PS) - Modul 6
    PS_NOISE_SATURATION = 0x02200001,     // Varning: Brus påverkar mätning
    
    // Motor Controller (MC) - Modul 7
    MC_STALL_DETECTED = 0x02100001,       // Kritisk: Motorstopp
    MC_OVER_CURRENT = 0x02100002,         // Kritisk: Överström
    MC_PID_WINDUP = 0x02200003,           // Varning: PID integrator windup
    
    // Display (DI) - Modul 8
    DI_NO_ERROR = 0x03000000              // Ingen fel
};

#endif // ERROR_CODES_H
