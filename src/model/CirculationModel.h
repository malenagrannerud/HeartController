/**
 * @file CirculationModel.h
 * @brief Circulation dynamics for TAH simulation
 */

#ifndef CIRCULATIONMODEL_H
#define CIRCULATIONMODEL_H

float calculateNewRAP(float actualAoP, float heartRate);
float calculateNewLAP(float actualPAP, float heartRate);

#endif