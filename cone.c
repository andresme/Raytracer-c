/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */
#include "cone.h"

//Devuelve un struct cone con todos los datos asociados.
cone* addCone(long double center[], long double vectorQ[], long double start[], long double p[]){
	cone *newCone = (cone *)malloc(sizeof(cone));
	vector *newCenter = (vector *)malloc(sizeof(vector));
	vector *newVectorQ = (vector *)malloc(sizeof(vector));

	newCenter->x = center[0];
	newCenter->y = center[1];
	newCenter->z = center[2];
	newCone->center = newCenter;

	newVectorQ->x = vectorQ[0];
	newVectorQ->y = vectorQ[1];
	newVectorQ->z = vectorQ[2];
	newCone->Q = newVectorQ;

	newCone->start = start[0];
	newCone->end = start[1];
	newCone->u = p[0];
	newCone->v = p[1];
	newCone->prop = pow(p[0]/p[1],2);

	return newCone;

}

//Calcula la normal del cono
vector* calcNormalCone(vector eye, vector dir, long double t, objectNode *this){

	cone *obj = (cone *) this->tipo;
	vector *N = (vector *)malloc(sizeof(vector));
	long double xi, yi, zi, x0, y0, z0, xq, yq, zq, xl, yl, zl, l, cosLQ, H;
	xi = eye.x + (t * dir.x);
	yi = eye.y + (t * dir.y);
	zi = eye.z + (t * dir.z);

	x0 = obj->center->x;
	y0 = obj->center->y;
	z0 = obj->center->z;

	xq = obj->Q->x;
	yq = obj->Q->y;
	zq = obj->Q->z;

	xl = xi-x0;
	yl = yi-y0;
	zl = zi-z0;

	l = sqrt(pow(xl,2)+pow(yl,2)+pow(zl,2));

	xl = xl/l;
	yl = yl/l;
	zl = zl/l;

	cosLQ = (xq*xl+yq*yl+zq*zl);
	H = l/cosLQ;

	N->x = xi-(x0+H*xq);
	N->y = yi-(y0+H*yq);
	N->z = zi-(z0+H*zq);

	l = sqrt(pow(N->x,2)+pow(N->y,2)+pow(N->z,2));

	N->x = N->x/l;
	N->y = N->y/l;
	N->z = N->z/l;

	return N;
}

//Calcula las intersecciones con el cono
intersection* calcIntersecCone(vector dir, vector anchor, objectNode *object){
	long double Xc,Yc,Zc, Xa,Ya,Za, Xd, Yd, Zd, Xq, Yq, Zq;
	long double a,b,c,d,e,f,g,h, Vx,Vy,Vz;
	intersection *intersec = NULL;
	cone *this = (cone *) object->tipo;
	long double B, Y, D, A;
	long double t1, t2;

	Xc = this->center->x;
	Yc = this->center->y;
	Zc = this->center->z;

	Xq = this->Q->x;
	Yq = this->Q->y;
	Zq = this->Q->z;

	Xa = anchor.x;
	Ya = anchor.y;
	Za = anchor.z;

	Xd = dir.x;
	Yd = dir.y;
	Zd = dir.z;

	a = (pow(Xq,2)*Xd)+(Xq*Yd*Yq)+(Xq*Zd*Zq)-Xd;
	b = (pow(Yq,2)*Yd)+(Xd*Xq*Yq)+(Yq*Zd*Zq)-Yd;
	c = (pow(Zq,2)*Zd)+(Xd*Xq*Zq)+(Yd*Yq*Zq)-Zd;
	d = (Xa-Xc);
	e = (Ya-Yc);
	f = (Za-Zc);
	g = (Xd*Xq)+(Yd*Yq)+(Zd*Zq);
	h = this->prop;

	A = pow(a,2)+pow(b,2)+pow(c,2)-(h*pow(g,2));

	B = (2*a*d*(pow(Xq,2)-1))+(2*a*Xq*((Yq*e)+(Zq*f)))
	+(2*b*e*(pow(Yq,2)-1))+(2*b*Yq*((Xq*d)+(Zq*f)))
	+(2*c*f*(pow(Zq,2)-1))+(2*c*Zq*((Xq*d)+(Yq*e)))
	-(h*2*g*d*Xq)-(h*2*g*e*Yq)-(h*2*g*f*Zq);

	Y = pow((pow(Xq,2)-1)*d,2)+pow(Xq*((Yq*e)+(Zq*f)),2)
	+pow((pow(Yq,2)-1)*e,2)+pow(Yq*((Xq*d)+(Zq*f)),2)
	+pow((pow(Zq,2)-1)*f,2)+pow(Zq*((Xq*d)+(Yq*e)),2)
	+(2*d*Xq*(pow(Xq,2)-1)*((Yq*e)+(Zq*f)))
	+(2*e*Yq*(pow(Yq,2)-1)*((Xq*d)+(Zq*f)))
	+(2*f*Zq*(pow(Zq,2)-1)*((Xq*d)+(Yq*e)))
	-(h*pow(Xq*d,2))-(h*pow(Yq*e,2))-(h*pow(Zq*f,2))
	-(h*2*d*e*Xq*Yq)-(h*2*d*f*Xq*Zq)-(h*2*e*f*Yq*Zq);

	D = pow(B,2) - (4*A*Y);

	if(D < 0.0005){
		return intersec;
	}
	//Determina si devolver t1 o t2
	else if(D > 0){
		t1 = ((-1 * B) + sqrt(D))/(2 * A);
		t2 = ((-1 * B) - sqrt(D))/(2 * A);
		if(t1 < t2 && t1 > 0.0005){
			intersec = (intersection*) malloc(sizeof(intersection));

			Vx = (long double) Xa + t1*Xd;
			Vy = (long double) Ya + t1*Yd;
			Vz = (long double) Za + t1*Zd;
			d = (Vx-this->center->x)*this->Q->x +
				(Vy-this->center->y)*this->Q->y +
				(Vz-this->center->z)*this->Q->z;
			if(d < this->end && d > this->start){
				intersec->object = object;
				intersec->t = t1;
			}
			else{
				Vx = (long double) Xa + t2*Xd;
				Vy = (long double) Ya + t2*Yd;
				Vz = (long double) Za + t2*Zd;
				d = (Vx-this->center->x)*this->Q->x +
					(Vy-this->center->y)*this->Q->y +
					(Vz-this->center->z)*this->Q->z;
				if(d < this->end && d > this->start){
					intersec->object = object;
					intersec->t = t2;
				}
			}
		}
		else if(t2 < t1 && t2 > 0.0005){
			intersec = (intersection*) malloc(sizeof(intersection));

			Vx = (long double) Xa + t2*Xd;
			Vy = (long double) Ya + t2*Yd;
			Vz = (long double) Za + t2*Zd;
			d = (Vx-this->center->x)*this->Q->x +
				(Vy-this->center->y)*this->Q->y +
				(Vz-this->center->z)*this->Q->z;
			if(d < this->end && d > this->start){
				intersec->object = object;
				intersec->t = t2;
			}
			else{
				Vx = (long double) Xa + t1*Xd;
				Vy = (long double) Ya + t1*Yd;
				Vz = (long double) Za + t1*Zd;
				d = (Vx-this->center->x)*this->Q->x +
					(Vy-this->center->y)*this->Q->y +
					(Vz-this->center->z)*this->Q->z;
				if(d < this->end && d > this->start){
					intersec->object = object;
					intersec->t = t1;
				}
			}
		}
	}
	return intersec;
}
