# SIMPLE TOTAL ARTIFICIAL HEART CONTROLLER SIMULATOR

## Aim
To model a system for a TAH to understand how 
- heart rate (input by user), PAP (affects LAP) and AoP (affects RAP) interact [1]  
- the Frank-Starling mechanism determines target pressures based on preload
- a TAH display can show real-time data to users

    [1] https://pmc.ncbi.nlm.nih.gov/articles/PMC12625488/

## Overview
| Parameter | Description |
|-----------|-------------|
| HR | Heart Rate (bpm) - user can adjust to simulate exercise or rest |
| RAP | Right Atrial Pressure (mmHg) - blood pressure entering right ventricle from body |
| PAP | Pulmonary Artery Pressure (mmHg) - blood pressure exiting right ventricle to lungs |
| LAP | Left Atrial Pressure (mmHg) - blood pressure entering left ventricle from lungs |
| AoP | Aortic Pressure (mmHg) - blood pressure exiting left ventricle to body |
| MCU | Microcontroller |
| RPM | Revolutions Per Minute (pump speed) |

### System Diagram

----------------------BODY----------------------------
↑ ↓
│ AoP (aorta) ||||| Sensor1: RAP
┌─────────────┴─────────────-┐ ┌─────────────┴─────────────┐
│ LEFT VENTRICLE │ │ RIGHT VENTRICLE │
│ Pumps blood from lungs→body│ │ Pumps body → lungs │
│ Motor2: follows setpoint │ │ Motor1: follows setpoint │
└─────────────┬─────────────-┘ └─────────────┬─────────────┘
||||| Sensor2: LAP ↓ PAP
↑ ↓
└───────────────────────LUNGS─────────────────────────┘






                 ----------------------BODY----------------------------
                 ↑                                                     ↓
                 │ AoP (aorta)                                       ||||| Sensor1: RAP
   ┌─────────────┴─────────────-┐                        ┌─────────────┴─────────────┐
   │ LEFT VENTRICLE             │                        │ RIGHT VENTRICLE           │
   │ Pumps blood from lungs→body│                        │ Pumps body → lungs        │
   │ Motor2: follows setpoint   │                        │ Motor1: follows setpoint  │
   └─────────────┬─────────────-┘                        └─────────────┬─────────────┘
               ||||| Sensor2: LAP                                      ↓ PAP 
                 ↑                                                     ↓
                 └───────────────────────LUNGS─────────────────────────┘
                                   

ONE MCU CONTROL UNIT
Loop(
    Reads sensors
    Sets setpoints to motors
)
    
## Build and Run
bash
cmake -B build -S . && cmake --build build && ./build/heart_controller

## Controls
s - start simulation
u - increase heart rate - simulate the human moving more
d - decrease heart rate - simulate the human moving less
q - quit

##  Features
- Real-time motor dynamics with first-order response
- Frank-Starling mechanism for RV and LV
- Pressure sensors with simulated noise
- Anti-suction protection
- Statistical summary with clinical assessment

## Project Structure
text
HeartController/
├── include/
│   ├── Constants.h
│   ├── PressureSensor.h
│   ├── StarlingMechanism.h
│   ├── Motor.h
│   ├── CirculationModel.h
│   └── Terminal.h
├── src/
│   ├── PressureSensor.cpp
│   ├── StarlingMechanism.cpp
│   ├── Motor.cpp
│   ├── CirculationModel.cpp
│   ├── Terminal.cpp
│   └── main.cpp
├── CMakeLists.txt
└── README.md

## Dependencies
CMake 3.10+
C++17 compatible compiler
pthread (for terminal input)

## Author
Malena Grannerud

