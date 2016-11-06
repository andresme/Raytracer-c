/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#include "sphere.h"
#include "polygon.h"


//Devuelve un struct sphere para ser agregado a los objetos.
sphere* addSphere(long double center[], long double radius, int cantCortes, plane* cortes){
	sphere *newSphere = (sphere *)malloc(sizeof(sphere));
	vector *centerS = (vector *)malloc(sizeof(vector));

	centerS->y = center[0];
	centerS->x = center[1];
	centerS->z = center[2];

	newSphere->center = centerS;
	newSphere->radius = radius;
	newSphere->radius2 = radius * radius;
	newSphere->cantCortes = cantCortes;
	newSphere->cortes = cortes;

	return newSphere;
}

//Calcula la normal de la esfera
vector* calcNormalS(vector eye, vector dir, long double t, objectNode *this){
	vector *N = (vector*) malloc(sizeof(vector));
	sphere *tSphere = (sphere*) this->tipo;
	long double Xi, Yi, Zi, radius;

	Xi = eye.x + (t * dir.x);
	Yi = eye.y + (t * dir.y);
	Zi = eye.z + (t * dir.z);

	radius = tSphere->radius;

	N->x = Xi - tSphere->center->x;
	N->y = Yi - tSphere->center->y;
	N->z = Zi - tSphere->center->z;

	N->x = (N->x)/radius;
	N->y = (N->y)/radius;
	N->z = (N->z)/radius;



	return N;
}

//Calcula la interseccion con la esfera
intersection* calcIntersecS(vector dir, vector anchor, objectNode *object){
	int contador;
	long double Xc,Yc,Zc, Xa,Ya,Za, Xd, Yd, Zd, r2;
	intersection *intersec = NULL;
	sphere *this = (sphere *) object->tipo;

	long double B, Y, D;
	long double t1, t2, *t;

	Xc = this->center->x;
	Yc = this->center->y;
	Zc = this->center->z;
	r2 = this->radius2;
	Xa = anchor.x;
	Ya = anchor.y;
	Za = anchor.z;
	Xd = dir.x;
	Yd = dir.y;
	Zd = dir.z;

	B = 2 * (((Xa-Xc)*Xd)+((Ya-Yc)*Yd)+((Za-Zc)*Zd));
	Y = pow(Xa-Xc,2) + pow(Ya-Yc,2) + pow(Za-Zc,2) - r2;

	D = pow(B,2) - (4 * Y);

	if(D < 0){
	 return intersec;
	}
	else if(D > 0){
		t1 = ((-1 * B) + sqrt(D))/2;
		t2 = ((-1 * B) - sqrt(D))/2; //El menor

		if(t2 < 0.0005 && t1 > 0.0005){
			intersec = (intersection*) malloc(sizeof(intersection));
			intersec->object = object;
			intersec->t = t1;
		}
		else if(t1 > 0.0005 && t2 > 0.0005){
			intersec = (intersection*) malloc(sizeof(intersection));
	 		intersec->object = object;
	 		intersec->t = t2;
		}
	}
	if (intersec != NULL && this->cantCortes > 0){
		for(contador = 0; contador < this->cantCortes; contador++){
			t = calcIntersecPlane0(dir,anchor,this->cortes[contador]);
			if(t != NULL){
				if(t2-t[3] < 0.005){
					intersec->t = t1;
				}
			}
		}
	}
	return intersec;
}
