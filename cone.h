/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#ifndef CONE_H_
#define CONE_H_

#include "structs.h"
#include <math.h>

cone* addCone(long double center[], long double vectorQ[], long double start[], long double p[]);
vector* calcNormalCone(vector eye, vector dir, long double t, objectNode *this);
intersection* calcIntersecCone(vector dir, vector anchor, objectNode *object);


#endif /* CONE_H_ */
