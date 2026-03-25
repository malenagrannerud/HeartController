
/**
 * @file frank_starling.h
 * @brief ISO 62304: Frank-Starling mekanism - Klass I medicinsk programvara
 * @version 1.0.0
 * 
 * Risknivå: Klass B (Måttlig risk)
 * Spårbarhet: SRS-FS-001 till SRS-FS-005
 * 
 * Frank-Starling lagen: Ökat inloppstryck (preload) → Ökat utloppstryck (afterload)
 * Detta är hjärtats inneboende regleringsmekanism.
 * 
 * @see ISO 62304 Section 5.2: Riskhantering
 * @see ISO 62304 Section 5.3: Mjukvaruutvecklingsplan
 */

#ifndef FRANK_STARLING_H
#define FRANK_STARLING_H

#include <cstdint>
#include "error_codes.h"

/**
 * @class FrankStarling
 * @brief Implementerar Frank-Starling mekanismen enligt ISO 62304
 * 
 * @design SRS-FS-001: Beräkna utloppstryck från inloppstryck
 * @design SRS-FS-002: Hantera mättnad vid höga tryck
 * @design SRS-FS-003: Validera inparametrar
 * @design SRS-FS-004: Spårbar loggning av fel
 * @design SRS-FS-005: Deterministic beteende för medicinsk certifiering
 */
class FrankStarling {
private:
    // Kurvpunkter (ISO 62304: Spårbara konstanter)
    static constexpr double PRELOAD_POINTS[4] = {0.0, 2.0, 10.0, 15.0};   // mmHg
    static constexpr double AFTERLOAD_POINTS[4] = {0.0, 18.0, 30.0, 30.0}; // mmHg
    static constexpr int NUM_POINTS = 4;
    static constexpr double MIN_PRELOAD = 0.0;      // mmHg
    static constexpr double MAX_PRELOAD = 40.0;     // mmHg
    static constexpr double MIN_AFTERLOAD = 0.0;    // mmHg
    static constexpr double MAX_AFTERLOAD = 30.0;   // mmHg
    
    // Spårbarhetsdata (ISO 62304 Section 5.4)
    uint32_t m_version;
    uint64_t m_calls;
    double m_last_input;
    double m_last_output;
    ErrorCode m_last_error;
    
    // Privata metoder
    double linearInterpolate(double x, double x1, double y1, double x2, double y2) const noexcept;
    bool validateInput(double inletPressure) const noexcept;
    
public:
    /**
     * @brief Konstruktor - initierar Frank-Starling mekanism
     * @param version Mjukvaruversion (spårbarhet)
     */
    explicit FrankStarling(uint32_t version = 0x01000000) noexcept;
    
    /**
     * @brief Beräkna önskat utloppstryck baserat på inloppstryck
     * @param inletPressure Inloppstryck i mmHg (0.0 - 40.0)
     * @return Önskat utloppstryck i mmHg (0.0 - 30.0)
     * 
     * @risk R-FS-001: Felaktigt utloppstryck kan leda till hemodynamisk instabilitet
     * @control C-FS-001: Validering av inparametrar
     * @control C-FS-002: Begränsning av utdata till fysiologiskt område
     */
    double calculateTargetPressure(double inletPressure) noexcept;
    
    /**
     * @brief Hämta senaste felkod
     * @return Senaste felkod
     */
    ErrorCode getLastError() const noexcept { return m_last_error; }
    
    /**
     * @brief Hämta antal anrop (för spårbarhet)
     * @return Antal anrop
     */
    uint64_t getCallCount() const noexcept { return m_calls; }
    
    /**
     * @brief Hämta version
     * @return Mjukvaruversion
     */
    uint32_t getVersion() const noexcept { return m_version; }
    
    /**
     * @brief Återställ spårbarhetsdata
     */
    void resetTraceability() noexcept;
};

#endif // FRANK_STARLING_H
