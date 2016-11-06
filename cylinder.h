/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "raytracer.h"
#include "structs.h"
#include <math.h>

cylinder* addCylinder(long double center[], long double vectorQ[], long double start[], long double r, long double vectorG[]);
vector* calcNormalCylinder(vector eye, vector dir, long double t, objectNode *this);
intersection* calcIntersecCylinder(vector dir, vector anchor, objectNode *object);
rgb* getTextureCylinder(int style, long double t, vector dir, vector anchor, objectNode *object);



#endif /* CYLINDER_H_ */
