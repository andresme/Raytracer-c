/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_


#include "structs.h"
#include "load.h"
#include "sphere.h"

void recorrerPantalla();
rgb* DeQueColor();
void saveFB();
intersection* firstIntersection(vector a, vector b);

vector* interToLight(vector *p, vector Xi);
void assocValues();
long double factAtt(long double c[], vector *p, vector Xi);
long double productoPunto(vector *a, vector *b);
long double distLight(vector *p, vector Xi);
void freeStructs();
rgb* getTexel(long double u, long double v, int d, int h, objectNode *object);

#endif /* RAYTRACER_H_ */
