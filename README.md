## Aim
To model a system for a TAH to understand how:
- HR (input by user), PAP (affects LAP) and AoP (affects RAP) interact [1]
- the Frank-Starling mechanism determines target pressures based on preload
- a display can show real-time data to users

[1] https://pmc.ncbi.nlm.nih.gov/articles/PMC12625488/

## Overview
| Parameter | Description |
|-----------|-------------|
| HR | Heart Rate (bpm) - user can adjust to simulate exercise or rest |
| RAP | Right Atrial Pressure (mmHg) - blood pressure entering right ventricle from body |
| PAP | Pulmonary Artery Pressure (mmHg) - blood pressure exiting right ventricle to lungs |
| LAP | Left Atrial Pressure (mmHg) - blood pressure entering left ventricle from lungs |
| AoP | Aortic Pressure (mmHg) - blood pressure exiting left ventricle to body |


The system simulates a Total Artificial Heart (TAH) with:
- **Right Ventricle**: Pumps blood from body to lungs (PAP)
- **Left Ventricle**: Pumps blood from lungs to body (AoP)
- **Two pressure sensors**: RAP (preload to right side) and LAP (preload to left side)
- **Two motors**: Follow setpoints based on Frank-Starling mechanism
- **User input**: Adjust heart rate (u/d) to simulate exercise/rest

## Control Loop (One micro controller unit)
1. Read sensors (RAP, LAP) with simulated noise
2. Calculate target pressures using Frank-Starling curves
3. Update motor speeds with first-order dynamics
4. Update circulation model based on actual pressures and heart rate
5. Display real-time data and collect statistics

## Controls
s	Start simulation
u	Increase heart rate
d	Decrease heart rate
q	Quit


## Build and Run
```bash
cmake -B build -S . && cmake --build build && ./build/heart_controller



