/*
 * quadrics.c
 *
 *  Created on: May 28, 2012
 *      Author: andres
 */

#include "quadric.h"

quadric* addQuadric(long double matriz[], int cantCortes, plane* cortes){
	quadric* newQuadric = (quadric *)malloc(sizeof(quadric));
	newQuadric->A = matriz[0];
	newQuadric->B = matriz[1];
	newQuadric->C = matriz[2];
	newQuadric->D = matriz[3];
	newQuadric->E = matriz[4];
	newQuadric->F = matriz[5];
	newQuadric->G = matriz[6];
	newQuadric->H = matriz[7];
	newQuadric->J = matriz[8];
	newQuadric->K = matriz[9];
	newQuadric->cantCortes = cantCortes;
	newQuadric->cortes = cortes;

	return newQuadric;
}

vector* calcNormalQuadric(vector eye, vector dir, long double t, objectNode *this){
	quadric *obj = (quadric *) this->tipo;
	vector *N = (vector *)malloc(sizeof(vector));

	long double xi, yi, zi, A, B, C, D, E, F, G, H, J;
	long double l;

	xi = eye.x + (t * dir.x);
	yi = eye.y + (t * dir.y);
	zi = eye.z + (t * dir.z);

	A = obj->A;
	B = obj->B;
	C = obj->C;
	D = obj->D;
	E = obj->E;
	F = obj->F;
	G = obj->G;
	H = obj->H;
	J = obj->J;

	N->x = (A*xi) + (D*yi) + (F*zi) + G;
	N->y = (D*xi) + (B*yi) + (E*zi) + H;
	N->z = (F*xi) + (E*yi) + (C*zi) + J;

	l = sqrt(pow(N->x,2)+pow(N->y,2)+pow(N->z,2));

	N->x = N->x/l;
	N->y = N->y/l;
	N->z = N->z/l;

	return N;
}

intersection* calcIntersecQuadric(vector dir, vector anchor, objectNode *object){
	intersection *intersec = NULL;
	quadric *this = (quadric *) object->tipo;

	long double A,B,C,D,E,F,G,H,J,K, a, b, y, xd, yd, zd, xe, ye, ze, d;
	long double t1, t2, t0, *t;
	int contador;
	A = this->A;
	B = this->B;
	C = this->C;
	D = this->D;
	E = this->E;
	F = this->F;
	G = this->G;
	H = this->H;
	J = this->J;
	K = this->K;

	xd = dir.x;
	yd = dir.y;
	zd = dir.z;

	xe = anchor.x;
	ye = anchor.y;
	ze = anchor.z;

	a = (A * pow(xd, 2)) + (B * pow(yd, 2))+ (C * pow(zd, 2))
			+ (2* ( (D*xd*yd) + (E*yd*zd) + (F*xd*zd) ));

	b = 2*((A*xe*xd)+(B*ye*yd)+(C*ze*zd)+(D*xe*yd)+(D*ye*xd)
		+(E*ye*zd)+(E*ze*yd)+(F*ze*xd)+(F*xe*zd)+(G*xd)+(H*yd)+(J*zd));

	y = (A*pow(xe, 2)) + (B*pow(ye, 2))+
		(C*pow(ze, 2)) + (2*((D*xe*ye) + (E*ye*ze) + (F*ze*xe)+
		(G*xe) + (H*ye) + (J*ze))) + K;

	d = pow(b, 2)-(4*a*y);

	if(d > 0.0005){
		t1 = ((-1 * b) + sqrt(d))/(2*a);
		t2 = ((-1 * b) - sqrt(d))/(2*a); //El menor
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
		if (intersec != NULL){
			for(contador = 0; contador < this->cantCortes; contador++){
				t = calcIntersecPlane0(dir,anchor,this->cortes[contador]);
				if(t != NULL){
					if(t2-t[3] < 0.005){
						intersec->t = t1;
					}
				}
			}
		}
	}
	else if(d < 0.0001 && d > -0.0001){
		t0 = (-b)/(2*a);
		intersec = (intersection*) malloc(sizeof(intersection));
		intersec->object = object;
		intersec->t = t0;

		if (intersec != NULL){
			for(contador = 0; contador < this->cantCortes; contador++){
				t = calcIntersecPlane0(dir,anchor,this->cortes[contador]);
				if(t != NULL){
					if(t0-t[3] < 0.005){
						return NULL;
					}
				}
			}
		}
	}

	return intersec;
}
