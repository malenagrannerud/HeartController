/**
 * @file CirculationModel.h
 * @brief Models TRUE, REAL values we cant measure in this simulation
 *
 * Calulates new RAP and LAP based on previous true values heart rate.
 * Returns the new values
 */
 
#ifndef CIRCULATIONMODEL_H
#define CIRCULATIONMODEL_H

float updateTrueRAP(float trueAOP, float HR);
float updateTrueLAP(float truePAP, float HR);

#endif