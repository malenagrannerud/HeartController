/**
 * @file CirculationModel.h
 * @brief Circulation dynamics for TAH simulation
 * 
 * Models how the body responds to pump output.
 * Based on Realheart paper: AoP affects venous return to right atrium,
 * PAP affects venous return to left atrium, and HR affects both.
 */

#ifndef CIRCULATIONMODEL_H
#define CIRCULATIONMODEL_H

/**
 * @brief Calculate new Right Atrial Pressure based on AoP and HR
 * 
 * Higher aortic pressure reduces venous return to right atrium.
 * Higher heart rate increases venous return.
 * 
 * Formula: RAP = DEFAULT_RAP - (actualAoP * AOP_EFFECT_ON_RAP) 
 *                + (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN
 * 
 * @param actualAoP Actual Aortic Pressure (mmHg)
 * @param heartRate Current heart rate (bpm)
 * @return float New Right Atrial Pressure (mmHg), clamped to [MIN_RAP, MAX_RAP]
 */
float calculateNewRAP(float actualAoP, float heartRate);

/**
 * @brief Calculate new Left Atrial Pressure based on PAP and HR
 * 
 * Higher pulmonary pressure reduces venous return to left atrium.
 * Higher heart rate increases venous return.
 * 
 * Formula: LAP = DEFAULT_LAP - (actualPAP * PAP_EFFECT_ON_LAP) 
 *                + (heartRate - DEFAULT_HR) * HR_EFFECT_ON_VENOUS_RETURN
 * 
 * @param actualPAP Actual Pulmonary Artery Pressure (mmHg)
 * @param heartRate Current heart rate (bpm)
 * @return float New Left Atrial Pressure (mmHg), clamped to [MIN_LAP, MAX_LAP]
 */
float calculateNewLAP(float actualPAP, float heartRate);

#endif