# HeartController# HeartController - TAH Right Ventricle Control System
## Översikt
Detta projekt simulerar en kontrollenhet för vänster kammare i ett totalt artificiellt hjärta (TAH). Systemet använder Starling-mekanismen för att reglera utflödestrycket baserat på inloppstrycket (preload).


## Frank-Starling simulering - steg för steg

### 1. Hjärtat genererar preload (inloppstryck)
`HeartModel` simulerar en naturlig tryckvariation:
- Baserat på hjärtfrekvens (puls)
- Varierar med sinuskurva (systole/diastole)
- Preload (från kroppen) ändras mellan ~4-8 mmHg
- Afterload (till lungorna) ändras mellan ~15-25 mmHg

### 2. Trycksensor mäter preload
`PressureSensor` lägger till brus (±0.1 mmHg) för att simulera verklig mätning

### 3. Starling algoritmen beräknar önskat utloppstryck
`FrankStarling::calculateTargetPressure()` mappar inlopp till utlopp:
Beskriver hjärtats förmåga att öka slagvolymen vid ökat fyllnadstryck https://en.wikipedia.org/wiki/Frank%E2%80%93Starling_law. Implementeringen följer kurvan:

| Preload (mmHg) | Afterload (mmHg) |
|----------------|------------------|
| 0              | 0                | EMERGENCY
| 2              | 18               |
| 10             | 30               |
| 15+            | 30 (mättnad)     | EMERGENCY

Uträknat tryck är **setpoint** - det tryck systemet ska uppnå.

### 4. Motorn ställer in önskat tryck
`MotorController` tar setpoint och styr motorn:
- `setTargetPressure()` sätter önskat utloppstryck
- `update()` simulerar motorrespons (fördröjning, tröghet)
- Motorn når inte setpoint direkt, utan med en fördröjning

### 5. Utloppssensor mäter faktiskt tryck
Mäter motorns faktiska utloppstryck med brus

### 6. Reglerfel beräknas
`error = target - actual` visar hur bra motorn följer setpoint

### 7. Display visar i realtid
- Tryckstaplar (inlopp, mål, utlopp)
- Reglerfel (skillnad mellan mål och faktiskt)
- Motorvarvtal och strömförbrukning
- Puls och simuleringstid

## Exempel 
1. Person vilar
- Sensorn mäter P_in=5 mmHg
- Önskar utloppstryck beräkas (Starling lagen): P_ut=22.5 mmHg 
- Motorn försöker nå 22.5 mmHg, det vill säga 22.5*
- Verkligt utloppstryck mäts: P_actual=21.8 mmHg
- Reglerfel = 0.7 mmHg

2. Person reser sig
- Sensor mäter nu P_in=10 mmHg


Efter 10 sekunder avslutas simuleringen och statistik visas.



## Projektstruktur
HeartController/
├── CMakeLists.txt 
├── README.md 
├── include/ 
│ ├── error_codes.h 
│ ├── frank_starling.h 
│ ├── starling_validator.h 
│ ├── pressure_converter.h 
│ ├── heart_model.h 
│ ├── pressure_sensor.h 
│ ├── motor_controller.h 
│ └── display.h 
├── src/ 
│ ├── frank_starling.cpp
│ ├── starling_validator.cpp
│ ├── pressure_converter.cpp
│ ├── heart_model.cpp
│ ├── pressure_sensor.cpp
│ ├── motor_controller.cpp
│ ├── display.cpp
│ └── main.cpp
└── tests/ 
    ├── CMakeLists.txt
    └── unit/
    └── test_starling_validator.cpp

## Bygg och kör
### Förutsättningar
- CMake 3.10 eller senare
- C++17 kompatibel kompilator (GCC, Clang)

### Bygg
mkdir build && cd build
cmake ..
make

### Kör
bash
git clone https://github.com/malenagrannerud/HeartController.git
cd HeartController
rm -rf build
mkdir build
cd build
cmake ..
make
./heart_controller

### Kör tester
bash
ctest --output-on-failure

#eller
./tests/starling_validator_test

## Testresultat
## StarlingValidator - Unit Tests
Test: validatePoint()     ✓ PASSED
Test: isMonotonic()       ✓ PASSED
Test: withinLimits()      ✓ PASSED
Test: validateCurve()     ✓ PASSED
Test: hasSaturation()     ✓ PASSED
Test: getGain()           ✓ PASSED
Passed: 7 | Failed: 0

## Komponenter
### Starling
Beräknar utloppstryck från inloppstryck enligt Starling kurvan.

### StarlingValidator
Validerar att kurvan är:
- Monotont ökande
- Inom fysiologiska gränser (0-40 mmHg inlopp, 0-30 mmHg utlopp)
- Har tillräckligt antal punkter (minst 2)

### PressureConverter
Konverterar mellan tryck (mmHg) och spänning (Volt) enligt pumpens karaktär

Sensitivitet: 9.7 mV/mmHg

## Felkoder
### Kodbeskrivning
FS_SUCCESS	Inget fel
FS_INVALID_PRELOAD	Ogiltigt inloppstryck
FS_SATURATION_WARNING	Mättnad uppnådd
CV_INVALID_POINTS	Otillräckligt antal punkter
CV_NON_MONOTONIC	Kurva är inte monoton
CV_OUT_OF_RANGE	Värde utanför tillåtet område
PC_VOLTAGE_SATURATION	Spänning mättad
PC_PRESSURE_CLIPPED	Tryck klippt

## Licens
Unlicence License

## Version
1.0.0
