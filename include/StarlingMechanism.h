/**
 * @file StarlingMechanism.h
 * @brief Frank-Starling mechanism for TAH simulation
 * 
 * Models how the heart adjusts output based on preload.
 * Higher preload (RAP/LAP) results in higher target pressure (PAP/AoP).
 * Based on Realheart paper with piecewise linear approximation.
 */

#ifndef STARLINGMECHANISM_H
#define STARLINGMECHANISM_H

/**
 * @brief Calculate target Pulmonary Artery Pressure from RAP
 * 
 * Frank-Starling mechanism for right ventricle.
 * Maps preload (RAP) to afterload (target PAP).
 * 
 * Piecewise linear function:
 * - RAP 0-2 mmHg:   target = RAP * 6.0
 * - RAP 2-6 mmHg:   target = 12 + (RAP-2) * 2.5
 * - RAP 6-12 mmHg:  target = 22 + (RAP-6) * 2.17
 * - RAP > 12 mmHg:  target = 35 (max)
 * 
 * @param rap Right Atrial Pressure (mmHg) - preload
 * @return float Target Pulmonary Artery Pressure (mmHg)
 */
float starlingRight(float rap);

/**
 * @brief Calculate target Aortic Pressure from LAP
 * 
 * Frank-Starling mechanism for left ventricle.
 * Maps preload (LAP) to afterload (target AoP).
 * 
 * Piecewise linear function:
 * - LAP 0-5 mmHg:   target = LAP * 14.0
 * - LAP 5-12 mmHg:  target = 70 + (LAP-5) * 4.29
 * - LAP 12-20 mmHg: target = 100 + (LAP-12) * 2.5
 * - LAP > 20 mmHg:  target = 120 (max)
 * 
 * @param lap Left Atrial Pressure (mmHg) - preload
 * @return float Target Aortic Pressure (mmHg)
 */
float starlingLeft(float lap);

#endif