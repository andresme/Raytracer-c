/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */
#ifndef DISC_H_
#define DISC_H_

#include "structs.h"

disc* addDisc(long double center[], long double N[], long double r, long double r2);
vector* calcNormalDisc(vector eye, vector dir, long double t, objectNode *this);
intersection* calcIntersecDisc(vector dir, vector anchor, objectNode *object);

#endif /* DISC_H_ */
