/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */


#include "structs.h"
#include "sphere.h"
#include "cone.h"
#include "disc.h"
#include "cylinder.h"
#include "polygon.h"
#include "quadric.h"

long double mint(long double a, long double b){
	if(a < b) return a;
	else return b;
}

//Crea un objeto en memoria
void createObject(objectNode *newObject, rgb *newColor, int type, long double color[], long double amb, long double ks, int kn, long double o1, long double o2){
	newColor->r = color[0];
	newColor->g = color[1];
	newColor->b = color[2];

	newObject->tipoO = type;
	newObject->color = newColor;
	newObject->kn = kn;
	newObject->amb = amb;
	newObject->ks = ks;
	newObject->o1 = o1;
	newObject->o2 = o2;
}

//Agrega un objeto a la lista
void addObject(objectNode *newObject){
	if(!objects){
		objects = newObject;
		objects->next = NULL;
	}
	else{
		objectNode *current = objects;
		while(current->next){
			current = current->next;
		}
		current->next = newObject;
		current->next->next = NULL;
	}
}

//Agrega una esfera
void addSphereO(int type, long double center[], long double radius, long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int cantCortes, vector* cortes, vector* nCortes){
	objectNode *newObject = (objectNode *) malloc(sizeof(struct object));

	rgb* newColor = (rgb*)malloc(sizeof(rgb));
	sphere *newSphere;
	plane* planosCorte;

	createObject(newObject, newColor, type, color, amb, ks, kn, o1, o2);

	planosCorte = calcPlanosDeCorte(cantCortes, cortes, nCortes);

	newSphere = addSphere(center, radius, cantCortes, planosCorte);
	newObject->tipo = (void *) newSphere;
	newObject->calcNormal = &calcNormalS;
	newObject->calcIntersection = &calcIntersecS;

	addObject(newObject);

}

//Agrega un Cono
void addConeO(int type, long double center[], long double vectorQ[], long double start[], long double p[], long double color[], long double amb, long double ks, int kn, long double o1, long double o2){
	objectNode *newObject = (objectNode *) malloc(sizeof(struct object));
	rgb* newColor = (rgb*)malloc(sizeof(rgb));
	cone *newCone;

	createObject(newObject, newColor, type, color, amb, ks, kn, o1, o2);
	newCone = addCone(center, vectorQ, start, p);
	newObject->tipo = (void *) newCone;

	newObject->calcNormal = &calcNormalCone;
	newObject->calcIntersection = &calcIntersecCone;

	addObject(newObject);
}

//Agrega un disco
void addDiscO(int type, long double center[], long double N[], long double r, long double r2, long double color[], long double amb, long double ks, int kn, long double o1, long double o2){
	objectNode *newObject = (objectNode *) malloc(sizeof(struct object));
	rgb* newColor = (rgb*)malloc(sizeof(rgb));
	disc *newDisc;

	createObject(newObject, newColor, type, color, amb, ks, kn, o1, o2);
	newDisc = addDisc(center, N, r, r2);
	newObject->tipo = (void *) newDisc;

	newObject->calcNormal = &calcNormalDisc;
	newObject->calcIntersection = &calcIntersecDisc;
	addObject(newObject);
}

//Agrega un cilindro
void addCylinderO(int type, long double center[], long double N[],long double G[], long double start[], long double r, long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int style){
	objectNode *newObject = (objectNode *) malloc(sizeof(struct object));
	rgb* newColor = (rgb*)malloc(sizeof(rgb));
	cylinder *newCylinder;

	createObject(newObject, newColor, type, color, amb, ks, kn, o1, o2);
	newCylinder = addCylinder(center, N, start, r, G);
	newObject->tipo = (void *) newCylinder;
	newObject->texture = style;
	newObject->calcNormal = &calcNormalCylinder;
	newObject->calcIntersection = &calcIntersecCylinder;
	newObject->getTexture = &getTextureCylinder;

	addObject(newObject);
}

void addPolygonO(int type, vector* vertices, int cantidad, long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int style, char* imag){
	objectNode *newObject = (objectNode *) malloc(sizeof(struct object));
	rgb* newColor = (rgb*)malloc(sizeof(rgb));
	polygon *newPolygon;

	createObject(newObject, newColor, type, color, amb, ks, kn, o1, o2);
	newPolygon = addPolygon(vertices, cantidad);
	newObject->tipo = (void *) newPolygon;
	newObject->texture = style;
	newObject->calcNormal = &calcNormalPolygon;
	newObject->calcIntersection = &calcIntersecPolygon;
	newObject->getTexture = &getTexturePolygon;
	newObject->texFile = (char *) malloc(6*sizeof(char));
	newObject->texFile[0] = imag[0];
	strcat(newObject->texFile, ".avs");

	addObject(newObject);
}

void addQuadricO(int type, long double matriz[], long double color[], long double amb, long double ks, int kn, long double o1, long double o2, int cantCortes, vector* cortes, vector* nCortes){
	objectNode *newObject = (objectNode *) malloc(sizeof(struct object));
	rgb* newColor = (rgb*)malloc(sizeof(rgb));
	quadric *newQuadric;
	plane* planosCorte;

	createObject(newObject, newColor, type, color, amb, ks, kn, o1, o2);
	planosCorte = calcPlanosDeCorte(cantCortes, cortes, nCortes);

	newQuadric = addQuadric(matriz, cantCortes, planosCorte);
	newObject->tipo = (void *) newQuadric;
	newObject->calcNormal = &calcNormalS;
	newObject->calcIntersection = &calcIntersecS;

	newObject->calcNormal = &calcNormalQuadric;
	newObject->calcIntersection = &calcIntersecQuadric;

	addObject(newObject);

}



//Agrega las luces a memoria
void addLight(long double center[], long double intensity, long double c[], long double color[]){
	lightNode *newLight = malloc(sizeof(struct light));
	vector *centerS = (vector *)malloc(sizeof(vector));
	rgb* newColor = (rgb*)malloc(sizeof(rgb));

	centerS->x = center[0];
	centerS->y = center[1];
	centerS->z = center[2];

	newColor->r = color[0];
	newColor->g = color[1];
	newColor->b = color[2];

	newLight->center = centerS;

	newLight->color = newColor;

	newLight->intensity = intensity;

	newLight->c[0] = c[0];
	newLight->c[1] = c[1];
	newLight->c[2] = c[2];

	if(!lights){
			lights = newLight;
			lights->next = NULL;
		}
		else{
			lightNode *current = lights;
			while(current->next){
				current = current->next;
			}
			current->next = newLight;
			current->next->next = NULL;
		}
}

plane* calcPlanosDeCorte(int cantVec, vector* puntos, vector* normales){
	plane* planoDeCorte = (plane*) malloc(cantVec*sizeof(plane));
	int contador;

	for(contador = 0; contador < cantVec; contador++)
		planoDeCorte[contador] = createPlane0(puntos[contador], normales[contador])[0];
	return planoDeCorte;
}
