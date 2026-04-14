/**
 @file CirculationModel.h
 @brief Models how the circulation system ("body" of the patient) responds to changes 
 in 1) AOP or PAP and 2) HR. 

 Calculates the signals the pressure sensors will measure: true RAP and LAP

 This is a collection of stateless functions and static variables.
 No state (variables) --> no need for a class. Pure mathematical transformations.
 */

#ifndef CIRCULATIONMODEL_H
#define CIRCULATIONMODEL_H

/**
 @brief Updates RAP based on changes in HR and AOP
 @param actualAoP Calculated from current RPM in @class Motor 
 @param HR User-set HR (bpm)
 @return New RAP (mmHg)

 If AoP increases, RAP should decrease (less blood returning to the heart)
 If HR increases, RAP should increase (more blood returning to the heart)
 */

float updateRAP(float actualAoP, float HR);


/**
 @brief Updates LAP based on changes in HR and PAP
 @param actualPAP Calculated from current RPM in @class Motor (an assumed knowlege about pump performance and pressure generation)
 @param HR User-set HR (bpm)
 @return New LAP (mmHg)
    If PAP increases-->LAP decrease (less blood return to the heart)
    If HR increases-->LAP increase (more blood return to the heart)
 */
float updateLAP(float actualPAP, float HR);

#endif