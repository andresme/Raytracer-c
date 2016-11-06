/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */


#include "load.h"

//Carga los settings de la escena
void loadSettings(char* sFile){
	char info[40], *attrib;
	int contador;

	FILE* fileptr = fopen(sFile, "rt");

	while(fgets(info, 40, fileptr) != NULL){ //Leer el archivo completo
		if(strlen(info) > 1){ //si el elemento leido no esta vacio
			attrib = strtok(info, ":");
			if(!strcmp(attrib,"height")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &lHeight);
			}
			else if(!strcmp(attrib,"width")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &lWidth);
			}
			else if(!strcmp(attrib,"Ia")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lIa);
			}
			else if(!strcmp(attrib,"anti")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &anti);
			}
			else if(!strcmp(attrib,"mirror")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &mirror);
			}
			else if(!strcmp(attrib,"background")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lBackground.r);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lBackground.g);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lBackground.b);
			}
			else if(!strcmp(attrib,"eye")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lEye.x);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lEye.y);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &lEye.z);
			}
			else if(!strcmp(attrib,"window")){
				for(contador = 0; contador < 4; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &lWindow[contador]);
				}
			}
		}
	}
	fclose(fileptr);
}
//Carga las luces de la escena
void loadLights(char* lFile){
	long double intensity, center[3], c[3],color[3];
	char info[40], *attrib;
	int contador;

	FILE* fileptr = fopen(lFile, "rt");

	while(fgets(info, 40, fileptr) != NULL){ //Leer el archivo completo
		if(strlen(info) > 1){ //si el elemento leido no esta vacio
			attrib = strtok(info, ":");
			if(!strcmp(attrib, "center")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &center[contador]);
				}
			}
			else if(!strcmp(attrib, "color")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &color[contador]);
				}
			}
			else if(!strcmp(attrib, "c")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &c[contador]);
				}
			}
			else if(!strcmp(attrib,"inten")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &intensity);
			}
			else if(!strcmp(attrib,"add"))
				addLight(center,intensity,c,color);
		}
	}
	fclose(fileptr);
}

//Carga los objectos de la escena
void loadObjects(char* oFile){
	vector *vertices, *nCorte;
	long double radius,amb,ks,color[3],center[3],Q[3],p[2],start[2], N[3], f1[3], f2[3], radius2, G[3];
	long double matriz[10];
	long double K, o1, o2;
	char info[40], *attrib, imag[1];
	int contador,type, kn, cantVertex, cantCortes, style;

	FILE* fileptr = fopen(oFile, "rt");

	while(fgets(info, 40, fileptr) != NULL){ //Leer el archivo completo
		if(strlen(info) > 1){ //si el elemento leido no esta vacio
			attrib = strtok(info, ":");
			//Obtener la informacion del archivo
			if(!strcmp(attrib, "center")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &center[contador]);
				}
			}
			//Tipos de figura
			else if(!strcmp(attrib,"sphere"))
				type = 1;
			else if(!strcmp(attrib,"cone"))
				type = 2;
			else if(!strcmp(attrib,"disc"))
				type = 3;
			else if(!strcmp(attrib,"cylinder"))
				type = 4;
			else if(!strcmp(attrib,"polygon")){
				type = 5;
				contador = 0;
			}
			else if(!strcmp(attrib,"elipse"))
				type = 6;
			else if(!strcmp(attrib,"quadric"))
				type = 7;
			else if(!strcmp(attrib, "color")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &color[contador]);
				}
			}
			else if(!strcmp(attrib, "N")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &N[contador]);
				}
			}
			else if(!strcmp(attrib, "Matriz")){
				for(contador = 0; contador < 10; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &matriz[contador]);
				}
			}
			else if(!strcmp(attrib, "Q")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &Q[contador]);
				}
			}
			else if(!strcmp(attrib, "texFile")){
					attrib = strtok(NULL, ":");
					sscanf(attrib, "%s", &imag[0]);
			}
			else if(!strcmp(attrib, "G")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &G[contador]);
				}
			}
			else if(!strcmp(attrib, "f1")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &f1[contador]);
				}
			}
			else if(!strcmp(attrib, "f2")){
				for(contador = 0; contador < 3; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &f2[contador]);
				}
			}
			else if(!strcmp(attrib, "prop")){
				for(contador = 0; contador < 2; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &p[contador]);
				}
			}
			else if(!strcmp(attrib, "start")){
				for(contador = 0; contador < 2; contador++){
					attrib = strtok(NULL, ":");
					sscanf (attrib, "%Lg", &start[contador]);
				}
			}
			else if(!strcmp(attrib,"amb")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &amb);
			}
			else if(!strcmp(attrib,"o1")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &o1);
			}
			else if(!strcmp(attrib,"o2")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &o2);
			}
			else if(!strcmp(attrib,"radius")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &radius);
			}
			else if(!strcmp(attrib,"K")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &K);
			}
			else if(!strcmp(attrib,"radius2")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &radius2);
			}
			else if(!strcmp(attrib,"ks")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &ks);
			}
			else if(!strcmp(attrib,"kn")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &kn);
			}
			else if(!strcmp(attrib,"cantV")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &cantVertex);
				vertices = (vector*) malloc(cantVertex*sizeof(vector));
				contador = 0;
			}
			else if(!strcmp(attrib,"style")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &style);
			}
			else if(!strcmp(attrib,"vertex")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &vertices[contador].y);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &vertices[contador].x);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &vertices[contador].z);
				contador++;
			}
			else if(!strcmp(attrib,"cantCortes")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%d", &cantCortes);
				if(cantCortes == 0){
					nCorte = NULL;
					vertices = NULL;
				}
				else{
					nCorte = (vector*) malloc(cantCortes*sizeof(vector));
					vertices = (vector*) malloc(cantCortes*sizeof(vector));
				}
				contador = 0;
			}
			else if(!strcmp(attrib,"nCorte")){
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &nCorte[contador].y);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &nCorte[contador].x);
				attrib = strtok(NULL, ":");
				sscanf (attrib, "%Lg", &nCorte[contador].z);
			}
			else if(!strcmp(attrib, "//")){
				attrib = strtok(NULL, "\n");
			}
			else if(!strcmp(attrib,"add")){
				switch(type){
				case 1:
					addSphereO(type,center,radius,color,amb, ks, kn, o1, o2, cantCortes, vertices, nCorte);
					break;
				case 2:
					addConeO(type, center, Q, start, p, color, amb, ks, kn, o1, o2);
					break;
				case 3:
					addDiscO(type, center, N, radius, radius2, color, amb, ks, kn, o1, o2);
					break;
				case 4:
					addCylinderO(type, center, Q, G, start, radius, color, amb, ks, kn, o1, o2, style);
					break;
				case 5:
					addPolygonO(type, vertices, cantVertex, color, amb, ks, kn, o1, o2, style, imag);
					break;
				case 7:
					addQuadricO(type, matriz, color, amb, ks, kn, o1, o2, cantCortes, vertices, nCorte);
					break;
				}
			}
		}
	}
	fclose(fileptr);
}
