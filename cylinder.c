/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#include "cylinder.h"


//Devuelve un struct Cylinder para ser agregado a la lista de objetos
cylinder* addCylinder(long double center[], long double vectorQ[], long double start[], long double r, long double vectorG[]){
	cylinder *newCylinder = (cylinder *)malloc(sizeof(cylinder));
	vector *newCenter = (vector *)malloc(sizeof(vector));
	vector *newVectorQ = (vector *)malloc(sizeof(vector));
	vector *newVectorG = (vector *)malloc(sizeof(vector));

	newCenter->x = center[0];
	newCenter->y = center[1];
	newCenter->z = center[2];
	newCylinder->center = newCenter;

	newVectorQ->x = vectorQ[0];
	newVectorQ->y = vectorQ[1];
	newVectorQ->z = vectorQ[2];
	newCylinder->Q = newVectorQ;

	newVectorG->x = vectorG[0];
	newVectorG->y = vectorG[1];
	newVectorG->z = vectorG[2];
	newCylinder->G = newVectorG;

	newCylinder->start = start[0];
	newCylinder->end = start[1];
	newCylinder->r2 = r*r;
	newCylinder->r = r;

	return newCylinder;

}

//Calcula la normal del cilindro
vector* calcNormalCylinder(vector eye, vector dir, long double t, objectNode *this){

	cylinder *obj = (cylinder *) this->tipo;
	vector *N = (vector *)malloc(sizeof(vector));
	long double xi, yi, zi, x0, y0, z0, xq, yq, zq, l, d;

	xi = eye.x + (t * dir.x);
	yi = eye.y + (t * dir.y);
	zi = eye.z + (t * dir.z);

	x0 = obj->center->x;
	y0 = obj->center->y;
	z0 = obj->center->z;

	xq = obj->Q->x;
	yq = obj->Q->y;
	zq = obj->Q->z;

	d = ((xi-x0)*xq)+((yi-y0)*yq)+((zi-z0)*zq);

	l = obj->r;
	N->x = (xi-(x0+(d*xq)))/l;
	N->y = (yi-(y0+(d*yq)))/l;
	N->z = (zi-(z0+(d*zq)))/l;


	return N;
}

//Calcula las intersecciones del cilindro
intersection* calcIntersecCylinder(vector dir, vector anchor, objectNode *object){
	long double Xc,Yc,Zc, Xa,Ya,Za, Xd, Yd, Zd, r, Xq, Yq, Zq;
	long double a,b,c,d,e,f, Vx,Vy,Vz;
	intersection *intersec = NULL;
	cylinder *this = (cylinder *) object->tipo;
	long double B, Y, D, A;
	long double t1, t2;

	Xc = this->center->x;
	Yc = this->center->y;
	Zc = this->center->z;

	r = this->r2;

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

	A = pow(a,2)+pow(b,2)+pow(c,2);

	B = (2*a*d*(pow(Xq,2)-1))+(2*a*Xq*((Yq*e)+(Zq*f)))
	+(2*b*e*(pow(Yq,2)-1))+(2*b*Yq*((Xq*d)+(Zq*f)))
	+(2*c*f*(pow(Zq,2)-1))+(2*c*Zq*((Xq*d)+(Yq*e)));

	Y = pow(((pow(Xq,2)-1)*d),2)+pow((Xq*((Yq*e)+(Zq*f))),2)
	+pow(((pow(Yq,2)-1)*e),2)+pow((Yq*((Xq*d)+(Zq*f))),2)
	+pow(((pow(Zq,2)-1)*f),2)+pow((Zq*((Xq*d)+(Yq*e))),2)
	+(2*d*Xq*(pow(Xq,2)-1)*((Yq*e)+(Zq*f)))
	+(2*e*Yq*(pow(Yq,2)-1)*((Xq*d)+(Zq*f)))
	+(2*f*Zq*(pow(Zq,2)-1)*((Xq*d)+(Yq*e)))-r;

	//Calcula discriminante
	D = pow(B,2)-(4*A*Y);

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
			if(this->end == -1.0f || (d < this->end && d > this->start)){
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
			if(this->end == -1.0f || (d < this->end && d > this->start)){
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
				if(this->end == -1.0f || (d < this->end && d > this->start)){
					intersec->object = object;
					intersec->t = t1;
				}
			}
		}

	}
	return intersec;
}

rgb* getTextureCylinder(int style, long double t, vector dir, vector anchor, objectNode *object){
	rgb* texel;
	vector* N;
	long double v, u, d, Vx, Vy, Vz;
	cylinder *this = (cylinder *) object->tipo;
	N = calcNormalCylinder(anchor,dir,t,object);
	u = productoPunto(N,this->G);
	printf("u: %Lg\n",u);
	u = acos(u);
	//long ppunto = (long) productoPunto(N,this->G);
	//long double alpha = (long double) acos(ppunto * 0.0174532925);
	//printf("angulo: %Lg\n", u);
	//u = (long double) alpha/(2*3.14159);
	u = (long double) u/(2*3.14159);
	Vx = (long double) anchor.x + t*dir.x;
	Vy = (long double) anchor.y + t*dir.y;
	Vz = (long double) anchor.z + t*dir.z;

	d = (Vx-this->center->x)*this->Q->x + (Vy-this->center->y)*this->Q->y +(Vz-this->center->z)*this->Q->z;
	v = (long double) (d - this->start)/(this->end-this->start);

	//texel = getTexel(u,v,object);
	//printf("u:%Lg V:%Lg\n",u,v);
	return texel;

}
