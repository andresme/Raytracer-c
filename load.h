/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 * 		Andres Morales E. 201016752
 * 		Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

#ifndef LOAD_H_
#define LOAD_H_

int lHeight,lWidth;
long double lWindow[4], lIa;
int anti, mirror;
rgb lBackground;
vector lEye;


void loadSettings(char *file);
void loadObjects(char *file);
void loadLights(char *file);

#endif /* LOAD_H_ */
