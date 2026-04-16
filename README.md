
# TOTAL ARTIFICIAL HEART CONTROLL SIMULATOR 

This project simulates the control mechanism for total artificial heart (TAH) using C++. It models the Frank-Starling mechanism, first-order motor dynamics, and provides real-time user interaction with live terminal display, to understand how a TAH controll system can look like.


## ABBREVIATIONS
| Parameter | Description |
|-----------|-------------|
| TAH |Total artificial heart|
| HR | Heart Rate [bpm]  |
| CO | Cardiac Output [L/min]  |
| RV | Right ventricle |
| RAP | Right Atrial Pressure [mmHg] - blood pressure entering right ventricle from body |
| PAP | Pulmonary Artery Pressure [mmHg] - blood pressure exiting right ventricle to lungs |
| LV | Left ventricle|
| LAP | Left Atrial Pressure [mmHg] - blood pressure entering left ventricle from lungs |
| AoP | Aortic Pressure [mmHg] - blood pressure exiting left ventricle to body |

## INTRODUCTION

A Total Artificial Heart (TAH) is a mechanical pump implanted to replace a failing human heart, the left and right ventricles and the four valves. It is designed to take over the pumping function of the native organ to restore blood circulation throughout the body. 

TAHs can be designed to emulate the Frank-Starling law of the heart, which is the intrinsic ability of the heart to increase its stroke volume in response to increased venous return [1]. While the natural heart uses muscle fiber stretching to achieve this, TAHs can mimic this behavior through passive filling mechanisms, where a higher filling volume leads to a more forceful or complete emptying of the artificial chamber, thus increasing cardiac output. The Frank Starling law must achieve two primary goals

1) Balance the Ventricles: The RV sends blood to the lungs, and the LV sends that same blood to the body. The Frank-Starling mechanism helps coordinate this.
   
2) Respond to Body Demands: When you exercise, the muscles squeeze blood back to your heart faster. The Frank-Starling mechanism allows your heart to respond instantly by contracting more forcefully, increasing cardiac output to meet the body's higher demand for oxygen.

Understanding how RAP, PAP, LAP, and AoP interact [2] helps engineers design better TAH controllers that mimic the body's natural autoregulation. This simulation demonstrates that a simple piecewise linear Starling curve combined with first-order motor dynamics can achieve physiological regulation.

[1] https://en.wikipedia.org/wiki/Frank%E2%80%93Starling_law

[2] https://pmc.ncbi.nlm.nih.gov/articles/PMC12625488/

### Aim
To model a TAH system to understand how:
- the Frank-Starling mechanism determines target cardiac output based on preload
- a display can looke like in a TAH to display real-time data to users

## METHOD
The system simulates a TAH with:
- **Right Ventricle**: Pumps blood from body to lungs (PAP)
- **Left Ventricle**: Pumps blood from lungs to body (AoP)
- **Two pressure sensors**: RAP (preload to right side) and LAP (preload to left side)
- **Two motors**: Follow setpoints based on Frank-Starling mechanism
- **User input**: Adjust heart rate (u/d) to simulate exercise/rest

<img width="1030" height="770" alt="Skärmavbild 2026-04-08 kl  14 12 52" src="https://github.com/user-attachments/assets/6118589e-115d-4843-adf9-d1715f6f13f0" />


### Control Loop Steps
1. Initialize: Set baseline HR to 72 BPM.
2. Read sensors: Read RAP and LAP sensors (with simulated noise).
3. Calculate: Determine required CO using the piecewise linear Starling curve.
4. Actuate: Adjust motor RPM to match the calculated output.
5. Update circulation model: based on actual pressures and heart rate
6. Display: Show real-time data and collect statistics

## Controls

| Key | Action |
|-----|--------|
| s | Start simulation |
| u | Increase heart rate |
| d | Decrease heart rate |
| q | Quit |



## Requirements
- CMake 3.10 or higher
- C++17 compatible compiler (GCC 9+ or Clang 10+)
- pthread (for terminal input)
- STL - standard library (vector, algorithm, numeric, thread)
- termios - terminal input handling on Linux (requires Linux or WSL)

### Build and Run
```bash
cmake -B build -S . && cmake --build build && ./build/heart_controller
```

### Code Structure

The project follows an MVC-like architecture:

- Model: Motor, PressureSensor, StarlingCurve, CirculationModel, Statistics - contains all data and business logic
- View: Terminal - handles all screen output (no logic)
- Controller: Simulation - runs the main loop, handles user input, coordinates Model and View
- Entry: main.cpp - creates the Simulation object and starts it


## RESULT
Column Descriptions



================ TAH CONTROL SYSTEM SYSTEM ================
 HR: 72 BPM 
-----------------------------------------------------------
RIGHT VENTRICLE (Pulmonary)     | LEFT VENTRICLE (Systemic)
-----------------------------------------------------------
 INLET (RAP):  12.0 mmHg         | INLET (LAP):  10.2 mmHg
 TARGET CO:    5.1 L/min         | TARGET CO:    5.1 L/min
 ACTUAL CO:    5.0 L/min         | ACTUAL CO:    5.2 L/min
 ERROR    :   -0.1 L/min         | ERROR    :   +0.1 L/min
                                 |
PUMP SPEED:   2450 RPM           | PUMP SPEED:   2800 RPM
OUTLET (PAP): 25.4 mmHg          | OUTLET (AOP): 95.1 mmHg
-----------------------------------------------------------




## FUTURE IMPROVEMENTS
### High priority
- Unit testing and validation of Model classes
- Add input validation and error handling  to manual terminal I/O
- Validate simulation results against real patient data
  
### Medium priority
- Performance optimization: latency, throughput, real-time profiling
- Medical device standards (IEC 62304, ISO 13485, ISO 14971)
- Set up CI/CD pipeline
- Add configuration file support
  
### Lower priority
- Replace terminal UI with a graphical interface (Qt)
- Add logging to file for post-simulation analysis







