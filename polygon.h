/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "structs.h"
#include "raytracer.h"
#include <stdlib.h>


polygon* addPolygon(vector* vertices, int cantidad);
vector* calcNormalPolygon(vector eye, vector dir, long double t, objectNode *this);
intersection* calcIntersecPolygon(vector dir, vector anchor, objectNode *object);
rgb* getTexturePolygon(int style, long double t, vector dir, vector anchor, objectNode *object);
long double* calcIntersecPlane(vector dir, vector eye, plane *plano);
long double* calcIntersecPlane0(vector dir, vector anchor, plane plano);
void flatVertex(polygon *newPolygon, int cantidad, vector *vertices);
int pointExists(long double* inter, objectNode *object);
plane* createPlane(vector *vertices);
plane* createPlane0(vector vertice, vector normal);


#endif /* POLYGON_H_ */
