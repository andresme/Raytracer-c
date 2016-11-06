/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#include "disc.h"

//Devuelve un struct Disc con todos sus datos para ser agregados como objeto
disc* addDisc(long double center[], long double N[], long double r, long double r2){
	disc *newDisc = (disc *) malloc(sizeof(disc));
	vector *newCenter = (vector *) malloc(sizeof(vector));
	vector *newNormal = (vector *) malloc(sizeof(vector));
	long double temp;

	newCenter->x = center[0];
	newCenter->y = center[1];
	newCenter->z = center[2];
	newDisc->center = newCenter;
	newDisc->A = N[0];
	newDisc->B = N[1];
	newDisc->C = N[2];
	newDisc->D = -(center[0] * N[0] + center[1] * N[1] + center[2] * N[2]);

	temp = sqrt(pow(newDisc->A,2)+pow(newDisc->B,2)+pow(newDisc->C,2));

	newNormal->x = N[0]/temp;
	newNormal->y = N[1]/temp;
	newNormal->z = N[2]/temp;
	newDisc->N = newNormal;

	newDisc->r = r * r;
	newDisc->r2 = r2 * r2;

	return newDisc;

}

//Calcula la normal del disco
vector* calcNormalDisc(vector eye, vector dir, long double t, objectNode *this){
	disc *obj = (disc *) this->tipo;
	vector *N = (vector *)malloc(sizeof(vector));
	long double a,b,c;

	a = obj->N->x;
	b = obj->N->y;
	c = obj->N->z;

	N->x = a;
	N->y = b;
	N->z = c;
	return N;
}

//Calcula la interseccion con el disco
intersection* calcIntersecDisc(vector dir, vector anchor, objectNode *object){
	intersection *intersec = NULL;
	disc *this = (disc *) object->tipo;
	long double Xd, Yd, Zd, t, Xi, Yi, Zi;
	long double A, B, C, D, E, F, temp;

	A = this->A;
	B = this->B;
	C = this->C;
	D = this->D;

	Xd = dir.x;
	Yd = dir.y;
	Zd = dir.z;

	F = A*Xd+B*Yd+C*Zd;

	if(F < 0.0005 && F > -0.0005)
		return intersec;

	E = A*anchor.x+B*anchor.y+C*anchor.z+D;
	t = -E/F;

	if(t < 0.0005) return intersec;

	Xi = anchor.x + (t * dir.x);
	Yi = anchor.y + (t * dir.y);
	Zi = anchor.z + (t * dir.z);

	temp = pow((this->center->x)-Xi,2) + pow((this->center->y)-Yi,2) + pow((this->center->z)-Zi,2);

	if(this->r > temp && this->r2 < temp){
		intersec = (intersection *) malloc(sizeof(intersection));
		intersec->object = object;
		intersec->t = t;
	}

	return intersec;
}
