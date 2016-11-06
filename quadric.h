/*
 * cuadrics.h
 *
 *  Created on: May 28, 2012
 *      Author: andres
 */

#ifndef CUADRICS_H_
#define CUADRICS_H_

#include "structs.h"

quadric* addQuadric(long double matriz[], int cantCortex, plane* cortes);
vector* calcNormalQuadric(vector eye, vector dir, long double t, objectNode *this);
intersection* calcIntersecQuadric(vector dir, vector anchor, objectNode *object);

#endif /* CUADRICS_H_ */
