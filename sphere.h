/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "structs.h"


sphere* addSphere(long double center[], long double radius, int cantCortes, plane* cortes);

intersection* calcIntersecS(vector dir, vector anchor, objectNode *object);
long double discriminanteS(long double B, long double Y);
vector* calcNormalS(vector eye, vector dir, long double t, objectNode *this);

#endif /* SPHERE_H_ */


