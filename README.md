# HeartController# HeartController - TAH Left Ventricle Control System
## Översikt
Detta projekt implementerar en kontrollenhet för vänster kammare i ett totalt artificiellt hjärta (TAH). Systemet använder Starling-mekanismen för att reglera utflödestrycket baserat på inloppstrycket (preload).

## Frank-Starling Mekanismen
Beskriver hjärtats förmåga att öka slagvolymen vid ökat fyllnadstryck. Implementeringen följer kurvan:

| Preload (mmHg) | Afterload (mmHg) |
|----------------|------------------|
| 0              | 0 |
| 2              | 18 |
| 10             | 30 |
| 15+            | 30 (mättnad) |

## Projektstruktur
HeartController/
├── CMakeLists.txt # Byggkonfiguration
├── README.md # Denna fil
├── include/ # Header-filer
│ ├── error_codes.h # Felkoder för spårbarhet
│ ├── frank_starling.h # Starling mekanism
│ ├── starling_validator.h # Validering av kurva
│ ├── pressure_converter.h # Tryck ↔ Spänning
│ ├── heart_model.h # Hjärtmodell
│ ├── pressure_sensor.h # Trycksensor
│ ├── motor_controller.h # Motorstyrning
│ └── display.h # Terminal-visualisering
├── src/ # Källkod
│ ├── frank_starling.cpp
│ ├── starling_validator.cpp
│ ├── pressure_converter.cpp
│ ├── heart_model.cpp
│ ├── pressure_sensor.cpp
│ ├── motor_controller.cpp
│ ├── display.cpp
│ └── main.cpp
└── tests/ # Enhetstester
├── CMakeLists.txt
└── unit/
└── test_starling_validator.cpp

## Bygg och kör
### Förutsättningar
- CMake 3.10 eller senare
- C++17 kompatibel kompilator (GCC, Clang)

### Bygga
mkdir build && cd build
cmake ..
make

### Kör
bash
./heart_controller

### Kör tester
bash
ctest --output-on-failure

#eller
./tests/starling_validator_test

### Testresultat
========================================
StarlingValidator - Unit Tests
========================================
Test: validatePoint()     ✓ PASSED
Test: isMonotonic()       ✓ PASSED
Test: withinLimits()      ✓ PASSED
Test: validateCurve()     ✓ PASSED
Test: hasSaturation()     ✓ PASSED
Test: getGain()           ✓ PASSED
========================================
Passed: 7 | Failed: 0
========================================

Komponenter
FrankStarling
Beräknar utloppstryck från inloppstryck enligt Starling kurvan.

StarlingValidator
Validerar att kurvan är:
- Monotont ökande
- Inom fysiologiska gränser (0-40 mmHg inlopp, 0-30 mmHg utlopp)
- Har tillräckligt antal punkter (minst 2)

PressureConverter
Konverterar mellan tryck (mmHg) och spänning (Volt) enligt pump

0 mmHg → 0.093 V
40 mmHg → 0.6815 V
Sensitivitet: 9.7 mV/mmHg

Felkoder
Kod	Beskrivning
FS_SUCCESS	Ingen fel
FS_INVALID_PRELOAD	Ogiltigt inloppstryck
FS_SATURATION_WARNING	Mättnad uppnådd
CV_INVALID_POINTS	Otillräckligt antal punkter
CV_NON_MONOTONIC	Kurva är inte monoton
CV_OUT_OF_RANGE	Värde utanför tillåtet område
PC_VOLTAGE_SATURATION	Spänning mättad
PC_PRESSURE_CLIPPED	Tryck klippt
Licens
MIT License

Version
1.0.0
