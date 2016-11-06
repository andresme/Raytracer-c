/*
 * Tarea Programada Ray Tracer Avanzado
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 *
 */

#include "polygon.h"
#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))


polygon* addPolygon(vector* vertices, int cantidad){
	polygon *newPolygon = (polygon *) malloc(sizeof(polygon));

	newPolygon->plano = createPlane(vertices);
	newPolygon->cantVertex = cantidad;
	newPolygon->rectangle = NULL;
	newPolygon->vertices = vertices;
	flatVertex(newPolygon, cantidad, vertices);

	return newPolygon;
}

void flatVertex(polygon *newPolygon, int cantidad, vector *vertices){
	vertex2D *newVertex = (vertex2D *) malloc(cantidad*sizeof(vertex2D));
	int i, a, b, c;
	a = abs(newPolygon->plano->N->x);
	b = abs(newPolygon->plano->N->y);
	c = abs(newPolygon->plano->N->z);
	if(a>b && a>c){
		for(i = 0; i < cantidad; i++){
			newVertex[i].U = vertices[i].y;
			newVertex[i].V = vertices[i].z;
			newPolygon->flag = 0;
		}
	}
	else if(b>a && b>c){
		for(i = 0; i < cantidad; i++){
			newVertex[i].U = vertices[i].x;
			newVertex[i].V = vertices[i].z;
			newPolygon->flag = 1;
		}
	}
	else{
		for(i = 0; i < cantidad; i++){
			newVertex[i].U = vertices[i].x;
			newVertex[i].V = vertices[i].y;
			newPolygon->flag = 2;
		}
	}
	newPolygon->vertices2D = newVertex;

}

vector* calcNormalPolygon(vector eye, vector dir, long double t, objectNode *this){
	polygon *obj = (polygon *) this->tipo;
	vector *N = (vector *)malloc(sizeof(vector));
	N->x = obj->plano->N->x;
	N->y = obj->plano->N->y;
	N->z = obj->plano->N->z;

	return N;
}

intersection* calcIntersecPolygon(vector dir, vector anchor, objectNode *object){
	intersection *intersec = NULL;
	polygon *this = (polygon *) object->tipo;
	long double *planeInter = calcIntersecPlane(dir, anchor, this->plano);
	if((planeInter) && (pointExists(planeInter, object))){
		intersec = (intersection*) malloc(sizeof(intersection));
		intersec->object = object;
		intersec->t = planeInter[3];
	}
	free(planeInter);
	return intersec;
}

long double* calcIntersecPlane(vector dir, vector anchor, plane *plano){
	plane *this = (plane *) plano;
	long double *intersec = NULL;
	long double Xd, Yd, Zd, t, Xi, Yi, Zi;
	long double A, B, C, D, E, F;

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

	if((t > -0.0005)&&(t < 0.0005))
		return intersec;

	Xi = anchor.x + (t * dir.x);
	Yi = anchor.y + (t * dir.y);
	Zi = anchor.z + (t * dir.z);

	intersec = (long double*) malloc(4*sizeof(long double));
	intersec[0] = Xi;
	intersec[1] = Yi;
	intersec[2] = Zi;
	intersec[3] = t;
	return intersec;
}

long double* calcIntersecPlane0(vector dir, vector anchor, plane plano){
	long double *intersec = NULL;
	long double Xd, Yd, Zd, t, Xi, Yi, Zi;
	long double A, B, C, D, E, F;

	A = plano.A;
	B = plano.B;
	C = plano.C;
	D = plano.D;

	Xd = dir.x;
	Yd = dir.y;
	Zd = dir.z;

	F = A*Xd+B*Yd+C*Zd;
	if(F < 0.0005 && F > -0.0005)
		return intersec;

	E = A*anchor.x+B*anchor.y+C*anchor.z+D;
	t = -E/F;

	if((t > -0.0005)&&(t < 0.0005))
		return intersec;

	Xi = anchor.x + (t * dir.x);
	Yi = anchor.y + (t * dir.y);
	Zi = anchor.z + (t * dir.z);

	intersec = (long double*) malloc(4*sizeof(long double));
	intersec[0] = Xi;
	intersec[1] = Yi;
	intersec[2] = Zi;
	intersec[3] = t;
	return intersec;
}

int pointExists(long double inter[], objectNode *object){
	polygon *this = (polygon *) object->tipo;
	vertex2D* vertices2D = this->vertices2D;

	int contador, contador_paredes;
	long double b,u,v,x1,y1,x2,y2,m;

	contador_paredes = 0;

	if(this->flag == 0){
		u = inter[1];
		v = inter[2];
	}
	else if(this->flag == 1){
		u = inter[0];
		v = inter[2];
	}
	else{
		u = inter[0];
		v = inter[1];
	}

	for(contador = 0; contador < this->cantVertex-1; contador++){

		x1 = vertices2D[contador].U - u;
		y1 = vertices2D[contador].V - v;
		x2 = vertices2D[contador+1].U - u;
		y2 = vertices2D[contador+1].V - v;

		if((x1 > 0.0005) && (x2 > 0.0005) && ((y1 < -0.0005 && y2 > 0.0005)||(y2 < -0.0005 && y1 > 0.0005)))
			contador_paredes++;
		else if(((x1 < -0.0005 && x2 > 0.0005)||(x2 < -0.0005 && x1 > 0.0005))&&((y1 < -0.0005 && y2 > 0.0005)||(y2 < -0.0005 && y1 > 0.0005))){
			if(!(x2-x1 > -0.0005 && x2-x1 < 0.0005)){
				m = (long double)(y2-y1)/(x2-x1);
				if(!(m> -0.0005 && m < 0.0005)){
					b = (long double)y1-(m*x1);
					long double result = -b/m;
					if(result > 0.0005)
						contador_paredes++;
				}
			}
		}
	}
	x1 = vertices2D[this->cantVertex-1].U - u;
	y1 = vertices2D[this->cantVertex-1].V - v;
	x2 = vertices2D[0].U - u;
	y2 = vertices2D[0].V - v;

	if((x1 > 0.0005) && (x2 > 0.0005) && ((y1 < -0.0005 && y2 > 0.0005)||(y2 < -0.0005 && y1 > 0.0005)))
		contador_paredes++;
	else if(((x1 < -0.0005 && x2 > 0.0005)||(x2 < -0.0005 && x1 > 0.0005))&&((y1 < -0.0005 && y2 > 0.0005)||(y2 < -0.0005 && y1 > 0.0005))){
		if(!(x2-x1 > -0.0005 && x2-x1 < 0.0005)){
			m = (long double)(y2-y1)/(x2-x1);
			if(!(m> -0.0005 && m < 0.0005)){
				b = (long double)y1-(m*x1);
				long double result = -b/m;
				if(result > 0.0005)
					contador_paredes++;
			}
		}
	}

	if(contador_paredes%2 == 1){
		return 1;
	}
	else return 0;

}


plane* createPlane(vector *vertices){
	plane *newPlane = (plane*) malloc(sizeof(plane));

	vector *newNormal = (vector *) malloc(sizeof(vector));
	vector *v = (vector *) malloc(sizeof(vector));
	vector *w = (vector *) malloc(sizeof(vector));

	long double a, b, c, d, L;


	v->x = vertices[1].x - vertices[0].x;
	v->y = vertices[1].y - vertices[0].y;
	v->z = vertices[1].z - vertices[0].z;

	w->x = vertices[2].x - vertices[0].x;
	w->y = vertices[2].y - vertices[0].y;
	w->z = vertices[2].z - vertices[0].z;

	a = (v->y * w->z) - (w->y * v->z);
	b = (w->x * v->z) - (v->x * w->z);
	c = (v->x * w->y) - (w->x * v->y);
	d = -((a*vertices[0].x)+(b*vertices[0].y)+(c*vertices[0].z));


	L= sqrt(pow(a,2)+pow(b,2)+pow(c,2));

	a = a/L;
	b = b/L;
	c = c/L;
	d = d/L;

	newNormal->x = a;
	newNormal->y = b;
	newNormal->z = c;

	newPlane->N = newNormal;

	newPlane->A = a;
	newPlane->B = b;
	newPlane->C = c;
	newPlane->D = d;

	free(v); free(w);
	return newPlane;
}

plane* createPlane0(vector vertice, vector normal){
	plane *newPlane = (plane*) malloc(sizeof(plane));

	vector *newNormal = (vector *) malloc(sizeof(vector));

	long double a, b, c, d, L;
	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = -((a*vertice.x)+(b*vertice.y)+(c*vertice.z));

	L= sqrt(pow(a,2)+pow(b,2)+pow(c,2));

	a = a/L;
	b = b/L;
	c = c/L;
	d = d/L;

	newNormal->x = a;
	newNormal->y = b;
	newNormal->z = c;

	newPlane->N = newNormal;

	newPlane->A = a;
	newPlane->B = b;
	newPlane->C = c;
	newPlane->D = d;

	return newPlane;
}

vector getMinimos(int cantVertex, vector *vertices){
	int contador;
	vector mins;

	mins.x = vertices[0].x;
	mins.y = vertices[0].y;
	mins.z = vertices[0].z;

	for(contador = 1; contador < cantVertex; contador++){
		if(vertices[contador].x < mins.x)
			mins.x = vertices[contador].x;
		if(vertices[contador].y < mins.y)
			mins.y = vertices[contador].y;
		if(vertices[contador].z < mins.z)
			mins.z = vertices[contador].z;
	}

	return mins;
}

vector getMaximos(int cantVertex, vector *vertices){
	int contador;
	vector maxs;

	maxs.x = vertices[0].x;
	maxs.y = vertices[0].y;
	maxs.z = vertices[0].z;

	for(contador = 1; contador < cantVertex; contador++){
		if(vertices[contador].x > maxs.x)
			maxs.x = vertices[contador].x;
		if(vertices[contador].y > maxs.y)
			maxs.y = vertices[contador].y;
		if(vertices[contador].z > maxs.z)
			maxs.z = vertices[contador].z;
	}

	return maxs;
}


void shapeRectangle(polygon* poli){
	vector *vertices = (vector*) malloc(4*sizeof(vector));
	poli->U = (vector*) malloc(sizeof(vector));
	poli->V = (vector*) malloc(sizeof(vector));

	vertices[0] = getMinimos(poli->cantVertex, poli->vertices);
	vertices[2] = getMaximos(poli->cantVertex, poli->vertices);

	vertices[1].x = vertices[0].x;
	vertices[1].y = vertices[2].y;
	vertices[1].z = vertices[2].z;

	vertices[3].x = vertices[2].x;
	vertices[3].y = vertices[0].y;
	vertices[3].z = vertices[0].z;

	poli->w = (long double) sqrt(pow((vertices[1].x-vertices[0].x),2)+pow((vertices[1].y-vertices[0].y),2)+pow((vertices[1].z-vertices[0].z),2));

	poli->U->x = (long double) (vertices[1].x-vertices[0].x)/poli->w;
	poli->U->y = (long double) (vertices[1].y-vertices[0].y)/poli->w;
	poli->U->z = (long double) (vertices[1].z-vertices[0].z)/poli->w;

	poli->h = (long double) sqrt(pow((vertices[3].x-vertices[0].x),2)+pow((vertices[3].y-vertices[0].y),2)+pow((vertices[3].z-vertices[0].z),2));

	poli->V->x = (long double) (vertices[3].x-vertices[0].x)/poli->h;
	poli->V->y = (long double) (vertices[3].y-vertices[0].y)/poli->h;
	poli->V->z = (long double) (vertices[3].z-vertices[0].z)/poli->h;

	poli->rectangle = vertices;
}

rgb* getTexturePolygon(int style, long double t, vector dir, vector anchor, objectNode *object){
	rgb* texel;
	polygon *this = (polygon *) object->tipo;
	long double u, v, d, h,  Xi, Yi, Zi;
	vector *I;

	I = (vector*) malloc(sizeof(vector));

	if(this->rectangle == NULL)
		shapeRectangle(this);

	Xi = (long double) anchor.x + (t * dir.x);
	Yi = (long double) anchor.y + (t * dir.y);
	Zi = (long double) anchor.z + (t * dir.z);

	I->x = Xi - this->rectangle[0].x;
	I->y = Yi - this->rectangle[0].y;
	I->z = Zi - this->rectangle[0].z;
	d = productoPunto(I,this->U);
	h = productoPunto(I,this->V);

	if(object->texture == 1){
		u = (long double) d/this->w;
		v = (long double) h/this->h;
	}
	else{
		u = 0;
		v = 0;
	}
	texel = getTexel(u,v,d,h,object);
	free(I);
	return texel;
}
