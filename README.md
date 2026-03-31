# HeartController# HeartController - TAH Right Ventricle Control System
## Översikt
// ============================================================================
// SIMPLIFIED TOTAL ARTIFICIAL HEART CONTROLLER
// ============================================================================
// ABBREVIATIONS:
//   HR  = Heart Rate (bpm) - user can adjust to simulate exercise or rest
//   RAP = Right Atrial Pressure (mmHg) - blood pressure entering right ventricle from body
//   PAP = Pulmonary Artery Pressure (mmHg) - blood pressure exiting right ventricle to lungs
//   LAP = Left Atrial Pressure (mmHg) - blood pressure entering left ventricle from lungs
//   AoP = Aortic Pressure (mmHg) - blood pressure exiting left ventricle to body
//
//   MCU = MICROCONTROLLER
//   RPM = Revolutions Per Minute (pump speed)
// ============================================================================
//
//                 ----------------------BODY----------------------------
//                 ↑                                                     ↓
//                 │ AoP (aorta)                                       ||||| Sensor1: RAP
//   ┌─────────────┴─────────────-┐                        ┌─────────────┴─────────────┐
//   │ LEFT VENTRICLE             │                        │ RIGHT VENTRICLE           │
//   │ Pumps blood from lungs→body│                        │ Pumps body → lungs        │
//   │ Motor2: follows setpoint   │                        │ Motor1: follows setpoint  │
//   │ Driver2: controls motor    │                        │ Driver1: controls motor   │
//   └─────────────┬─────────────-┘                        └─────────────┬─────────────┘
//               ||||| Sensor2: LAP                                      ↓ PAP 
//                 ↑                                                     ↓
//                 └───────────────────────LUNGS─────────────────────────┘
//                                    
//      MCU: 
//      Loop
//          Reads sensors
//          Sets setpoints to motors
//               
// 
// 
// AIM: To model a system for a TAH to understand how 
//      i: HR (input by user), PAP (affects LAP) and AoP (affects RAP) interact  
//      ii: how the Frank-Starling mechanism determines target pressures based on preload
//      iii: how a display can show real-time data to users
//
// The system is programmed to respond to changes in RAP and LAP by adjusting RPM
// to maintain physiological pressures, just like a natural heart follows 
// the Frank-Starling mechanism.
// The user can adjust HR to simulate exercise or rest, and the system will show 
// how the pumps respond to maintain circulation.
// ============================================================================
// ONE MCU CONTROL LOOP
// ============================================================================
//
// Each time step (DT = 50 ms):
//
//   1. Read user input (u/d/q) via USB serial
//      Update HR if key pressed
//
//   2. Read sensors:
//      RAP = analogRead(A0) → convert to mmHg
//      LAP = analogRead(A1) → convert to mmHg
//
//   3. Right ventricle control:
//      targetPAP = starling(RAP)
//      setpointRPM1 = pressureToRPM(targetPAP)
//      setpointVoltage1 = rpmToVoltage(setpointRPM1)
//      updateMotor1(setpointRPM1, DT)
//      actualPAP = motor1.getPressure()
//      errorRight = targetPAP - actualPAP
//
//   4. Left ventricle control:
//      targetAoP = starling(LAP)
//      setpointRPM2 = pressureToRPM(targetAoP)
//      setpointVoltage2 = rpmToVoltage(setpointRPM2)
//      updateMotor2(setpointRPM2, DT)
//      actualAoP = motor2.getPressure()
//      errorLeft = targetAoP - actualAoP
//
//   5. Circulation model:
//      newRAP = calculatePreload(actualAoP, HR)  // AoP affects RAP
//      newLAP = calculatePreload(actualPAP, HR)  // PAP affects LAP
//
//   6. Anti-suction check:
//      if RAP < 2 mmHg → reduce motor1 speed
//      if LAP < 2 mmHg → reduce motor2 speed
//
//   7. Print data row to serial monitor
//
// ============================================================================
//  8. Software includes an anti-suction algorithm:
// If inlet pressure drops suddenly or if pump power consumption starts fluctuating
// (sign of turbulence), the system immediately lowers speed to let the atrium refill.
//







## Licens
Unlicence License

## Version
1.0.0
