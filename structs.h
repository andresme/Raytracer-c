/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <malloc.h>
#include <math.h>

//Estructura vector, tambien usada como punto
typedef struct vector{
	long double x,y,z;
} vector;

//Estructura plano
typedef struct plane{
	vector *N;
	long double A, B, C, D;
} plane;

//estructura de rgb (para el color).
typedef struct rgb{
	long double r, g, b;
} rgb;

//estructura para almacenar objetos
typedef struct object{
	char *texFile;
	int tipoO, kn, texture;//texture: 0 = no tiene, 1 = stretched, 2 = tiled
	void *tipo; // puntero para la informacion especifica del objeto
	rgb* color;
	long double amb, ks, o1, o2;
	rgb* (*getTexture) (int style, long double t, vector dir, vector anchor, struct object *this);
	vector* (*calcNormal) (vector eye, vector dir, long double t, struct object *this);
	struct intersection* (*calcIntersection)(vector dir, vector anchor, struct object *object);
	struct object *next;
} objectNode;

//Interseccion, un t y un objeto.
typedef struct intersection{
	long double t;
	objectNode *object;
	struct intersection *next;
} intersection;

//Datos especificos de la esfera
typedef struct sphere{
	vector* center;
	plane* cortes;
	long double radius;
	long double radius2;
	int cantCortes;
} sphere;

//Datos del cono
typedef struct cone{
	vector* center;
	vector* Q;
	long double start, end;
	long double u, v;
	long double prop;
} cone;

//Datos del cilindro
typedef struct cylinder{
	vector* center;
	vector* Q, *G;
	long double start, end;
	long double r;
	long double r2;
} cylinder;

//Datos del disco
typedef struct disc{
	vector *center;
	vector *N;
	long double r, r2, A, B, C, D;
} disc;

typedef struct vertex2D{
	long double U,V;
} vertex2D;

typedef struct polygon{
	int cantVertex, flag;
	long double w, h;
	vector *vertices, *rectangle, *U, *V;
	plane *plano;
	vertex2D *vertices2D;
} polygon;

typedef struct quadric{
	long double A,B,C,D,E,F,G,H,J,K;
	plane* cortes;
	int cantCortes;
}quadric;

//Datos de la luz
typedef struct light{
	vector* center;
	long double intensity;
	long double c[3];
	rgb* color;
	struct light *next;
}lightNode;

objectNode *objects;
lightNode *lights;

plane* calcPlanosDeCorte(int cantVec, vector* puntos, vector* normales);

void addSphereO(int type, long double center[], long double radius, long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int cantCortes, vector* cortes, vector* nCortes);
void addConeO(int type, long double center[], long double vectorQ[], long double start[], long double p[], long double color[], long double amb, long double ks, int kn, long double o1, long double o2);
void addDiscO(int type, long double center[], long double N[], long double r, long double r2, long double color[], long double amb, long double ks, int kn, long double o1, long double o2);
void addLight(long double center[], long double intensity, long double c[], long double color[]);
void addCylinderO(int type, long double center[], long double N[],long double G[], long double start[], long double r, long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int style);
void addPolygonO(int type, vector* vertices, int cantidad, long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int style, char *imag);
void addQuadricO(int type, long double matriz[], long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int cantCortes, vector* cortes, vector* nCortes);
long double mint(long double a, long double b);

#endif /* STRUCTS_H_ */


