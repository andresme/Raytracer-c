/*
 * Tarea Programada Ray Tracer Simple
 * Por:
 *      Andres Morales E. 201016752
 *      Sebastian Ramirez R. 201052816
 *
 * Curso: Computer Graphics.
 */


#include "raytracer.h"
#include <omp.h>

//Macro para invertir bytes para guardar en formato avs.
#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))

#define CHUNKSIZE 40

//Datos generales de la escena
long double window[4], Ia;
int alto, ancho;
vector eye;
rgb backGround, **frameBuffer;
int antiLvl;
int mirrorLvl;

//Inicia el raytracer
int main(int argc, char **argv){

    //Carga objetos.
    loadLights("lights.txt");
    loadObjects("shapes.txt");
    loadSettings("settings.txt");
    assocValues();
    //Empieza a generar la escena
    recorrerPantalla();
    //Guarda el FrameBuffer
    saveFB();
    //Libera memoria utilizada
    freeStructs();
    system("convert imagen.avs imagen.png");
    system("display imagen.png");
    return 0;
}

//Asocia valores de archivo a variables.
void assocValues(){
    int contador;

    alto = lHeight;
    ancho = lWidth;
    Ia = lIa;
    backGround = lBackground;
    eye = lEye;
    antiLvl = anti;
    mirrorLvl = mirror;
    frameBuffer = (rgb**)malloc(alto*sizeof(rgb*));

    for(contador = 0; contador < 4; contador++)
        window[contador] = lWindow[contador];

    for(contador = 0; contador < alto; contador++)
        frameBuffer[contador]= (rgb*)malloc(ancho*sizeof(rgb));
}

int colorDiferente(rgb *Array, int cont, rgb *actual){
    long double r,g,b,r1,g1,b1, dr, dg, db;
    r1 = Array[cont-1].r;
    g1 = Array[cont-1].g;
    b1 = Array[cont-1].b;
    r = actual->r/cont;
    g = actual->g/cont;
    b = actual->b/cont;
    if(cont >= antiLvl)
        return 0;
    else{
        dr = sqrt(pow(r-r1,2));
        dg = sqrt(pow(g-g1,2));
        db = sqrt(pow(b-b1,2));
        if(dr > 0.0003 || dg > 0.0003 || db > 0.0003){
            return 1;
        }
    }
    return 0;
}

//Funcion principal
void recorrerPantalla(){
    /*
         * i, j = pixel del FrameBuffer;
         * Xw, Yw, Zw = Coordenada de la ventana
         * d = vector de direccion.
     */
    int i, j, cont, cont2;
    long double L, Xw, Yw, Zw, AAI, AAJ, sum;
    rgb *color, *colorTemp, *colorTempArray;
    vector d;
    #pragma omp parallel private(i,j,Xw,Yw,Zw,color,L,d, AAI, AAJ, cont, colorTemp, sum, colorTempArray, cont2)
    {
        #pragma omp for schedule(dynamic, CHUNKSIZE)
        for(i = 0; i < alto; i++){
            for(j = 0; j < ancho; j++){
                colorTemp = (rgb *) malloc(sizeof(rgb));
                colorTempArray = (rgb *) malloc(antiLvl*sizeof(rgb));
                color =(rgb *) malloc(sizeof(rgb));
                color->r = 0.0;
                color->b = 0.0;
                color->g = 0.0;
                cont = 0;
                cont2 = 0;
                sum = 1;

                do{
                    cont = 0;
                    for(AAI = 0; AAI <= 1; AAI += sum){
                        for(AAJ = 0; AAJ <= 1; AAJ += sum){
                            Xw = ((long double)(i + AAI) * (window[1] - window[0]) / alto) + window[0];
                            Yw = ((long double)(j + AAJ) * (window[3] - window[2]) / ancho) + window[2];
                            Zw = 0.0;

                            d.x = (Xw - eye.x);
                            d.y = (Yw - eye.y);
                            d.z = (Zw - eye.z);

                            L = sqrt(pow(d.x,2) + pow((d.y),2) + pow((d.z),2));

                            d.x = d.x/L;
                            d.y = d.y/L;
                            d.z = d.z/L;

                            colorTemp = DeQueColor(d, eye, mirrorLvl);
                            colorTempArray[cont].r = colorTemp->r;
                            colorTempArray[cont].g = colorTemp->g;
                            colorTempArray[cont].b = colorTemp->b;
                            color->r += colorTemp->r;
                            color->g += colorTemp->g;
                            color->b += colorTemp->b;
                            cont++;
                        }
                    }
                    sum *= 0.5;
                    cont2 += cont;
                }while(colorDiferente(colorTempArray, cont, color));
                free(colorTempArray);
                color->r /= cont2;
                color->g /= cont2;
                color->b /= cont2;

                frameBuffer[i][j].r = color->r;
                frameBuffer[i][j].g = color->g;
                frameBuffer[i][j].b = color->b;
                free(color);
            }
        }
    }

}

long double expTo(long double num, int exp){
    int i;
    long double res = num;
    for(i =  0; i < exp; i++)
        res = res * num;
    return res;
}
long double min(long double a, long double b){
    if(a < b)
        return a;
    else
        return b;
}

//Devuelve el punto de interseccion.
vector *getXi(intersection* FI, vector a, vector* b){
    vector* L = (vector*) malloc(sizeof(vector));
    long double Xi, Yi, Zi;

    Xi = eye.x + (FI->t*a.x);
    Yi = eye.y + (FI->t*a.y);
    Zi = eye.z + (FI->t*a.z);

    L->x = Xi;
    L->y = Yi;
    L->z = Zi;
    return L;
}


//Determina el color del objeto.
rgb* DeQueColor(vector a, vector b, int mirrorLvl){
    /*
         * a = vector direccion, b = ancla.
         * N = Normal del objeto.
         * L = Vector del objeto a la luz.
         * Xi = Punto de interseccion.
         * R = Vector reflejo de L con respecto a N.
         * Temp = punto interseccion.
         * Temp2 = vector que apunta del objeto a la luz .
         * temp3 = vector de direccion.
         * nXd = vector de direccion que apunta de nuevo al ojo.
    */
    intersection* First_Intersection, *shadow_Intersection;
    rgb *color, *colorTemp;
    vector *N, *L, *Xi, *nXd, *R, Temp, Temp2, *temp3;
    lightNode *k;
    long double I, E, fact_Att, producto;


    nXd = (vector*) malloc(sizeof(vector));
    temp3 = (vector*) malloc(sizeof(vector));
    nXd->x = -a.x;
    nXd->y = -a.y;
    nXd->z = -a.z;

    temp3->x = a.x;
    temp3->y = a.y;
    temp3->z = a.z;

    First_Intersection = firstIntersection(a, b);
    if(!First_Intersection){
        color = (rgb*) malloc(sizeof(rgb));
        color->r = backGround.r;
        color->g = backGround.g;
        color->b = backGround.b;
    }
    else{
        R = (vector*) malloc(sizeof(vector));
        color = (rgb*) malloc(sizeof(rgb));
        N = First_Intersection->object->calcNormal(b, a, First_Intersection->t, First_Intersection->object);
        if(productoPunto(temp3,N) > 0.0005){
            N->x = -N->x;
            N->y = -N->y;
            N->z = -N->z;
        }
        E = 0.0;
        I = 0.0;
        k = lights;
        while(k){
            Xi = getXi(First_Intersection, a, k->center);
            Temp2.x = Xi->x;
            Temp2.y = Xi->y;
            Temp2.z = Xi->z;
            L = interToLight(k->center, Temp2);
            fact_Att = factAtt(k->c, k->center, Temp2);
            producto = productoPunto(N,L);
            if(producto > 0.0){
                Temp.x = L->x;
                Temp.y = L->y;
                Temp.z = L->z;
                shadow_Intersection = firstIntersection(Temp, Temp2);
                if(!shadow_Intersection || shadow_Intersection->t > distLight(k->center, Temp2)){
                    I = I + (producto * First_Intersection->object->amb * fact_Att * k->intensity);
                    R->x = (N->x * 2 * producto)- L->x;
                    R->y = (N->y * 2 * producto)- L->y;
                    R->z = (N->z * 2 * producto)- L->z;
                    E = E + (expTo(productoPunto(R,nXd), First_Intersection->object->kn) * fact_Att * k->intensity) * First_Intersection->object->ks;
                }
                free(shadow_Intersection);

            }

            k = k->next;
        }
        I = I + Ia*First_Intersection->object->amb;
        I = min(1, I);
        E = min(1, E);
        if(First_Intersection->object->texture > 0){
                color = First_Intersection->object->getTexture(First_Intersection->object->texture, First_Intersection->t, a, b, First_Intersection->object);
                    color->r = I * color->r;
                    color->g = I * color->g;
                    color->b = I * color->b;
                }
                else{
                    color->r = I * First_Intersection->object->color->r;
                    color->g = I * First_Intersection->object->color->g;
                    color->b = I * First_Intersection->object->color->b;
                }

        if(E>0){
            color->r = color->r + E*(1-color->r);
            color->g = color->g + E*(1-color->g);
            color->b = color->b + E*(1-color->b);
        }
        if(First_Intersection->object->o2 > 0.0005 && mirrorLvl > 0){
            colorTemp = (rgb *) malloc(sizeof(rgb));
            producto = productoPunto(N,temp3);
            Temp.x = temp3->x - ((2 * producto) * N->x);
            Temp.y = temp3->y - ((2 * producto) * N->y);
            Temp.z = temp3->z - ((2 * producto) * N->z);
            Temp2.x = Xi->x;
            Temp2.y = Xi->y;
            Temp2.z = Xi->z;
            colorTemp = DeQueColor(Temp, Temp2, mirrorLvl - 1);
            color->r = First_Intersection->object->o1 * color->r +
                First_Intersection->object->o2 * colorTemp->r;

            color->g = First_Intersection->object->o1 * color->g +
                First_Intersection->object->o2 * colorTemp->g;

            color->b = First_Intersection->object->o1 * color->b +
                First_Intersection->object->o2 * colorTemp->b;
            free(colorTemp);
        }
        free(R);
        free(N);
        free(Xi);
    }
    free(First_Intersection);
    free(nXd);
    free(temp3);
    return color;
}

//Devuelve la interseccion mas proxima con algun objeto
intersection* firstIntersection(vector a, vector b){
    /*
         *a = vector direccion.
         *b = ancla
         *this = objeto actual
         *intersec, intersecTemp = intersecciones para saber cual gana.
     */
    objectNode *this = objects;
    intersection *intersec = NULL;
    intersection *intersecTemp = NULL;
    long double t = -1.0f;
    while(this){
        intersecTemp = this->calcIntersection(a, b, this);

        if(intersecTemp && t == -1.0f && intersecTemp->t > 0.0005){
            free(intersec);
            intersec = intersecTemp;
            t = intersec->t;
        }
        else if(intersecTemp && intersecTemp->t < t && intersecTemp->t > 0.0005){
            free(intersec);
            intersec = intersecTemp;
            t = intersec->t;
        }
        else{
            free(intersecTemp);
        }
        this = this->next;
    }
    return intersec;
}

//La distancia a la luz
long double distLight(vector *p, vector Xi){
    vector* L = (vector*) malloc(sizeof(vector));
    long double dist;

    L->x = p->x - Xi.x;
    L->y = p->y - Xi.y;
    L->z = p->z - Xi.z;

    dist = sqrt(pow(L->x,2) + pow(L->y,2) + pow(L->z,2));
    free(L);
    return dist;
}

//El vector que apunta a la luz desde el objeto
vector* interToLight(vector *p, vector Xi){
    vector* L = (vector*) malloc(sizeof(vector));
    long double dist;

    L->x = p->x - Xi.x;
    L->y = p->y - Xi.y;
    L->z = p->z - Xi.z;

    dist = sqrt(pow(L->x,2) + pow(L->y,2) + pow(L->z,2));

    L->x = (L->x)/dist;
    L->y = (L->y)/dist;
    L->z = (L->z)/dist;

    return L;
}

//Factor de atenuacion de las luces
long double factAtt(long double c[], vector *p, vector Xi){
    vector* L = (vector*) malloc(sizeof(vector));
    long double fact_Att;
    long double dist;

    L->x = p->x - Xi.x;
    L->y = p->y - Xi.y;
    L->z = p->z - Xi.z;

    dist = sqrt(pow(L->x,2) + pow(L->y,2) + pow(L->z,2));

    free(L);
    fact_Att = 1/(c[0]+(c[1]*dist)+(c[2]*pow(dist,2)));

    return min(1, fact_Att);
}

//El producto punto de dos vectores
long double productoPunto(vector *a, vector *b){
    long double result;
    result = a->x*b->x + a->y*b->y + a->z*b->z;
    return result;
}

//Guarda el frameBuffer a un archivo.avs
void saveFB(){
    int i,j;
    FILE *fptr;
    int width,height;

    if ((fptr = fopen("imagen.avs","w")) != NULL) {

        width = ancho;
        height = alto;

        width = FIX(width);
        height = FIX(height);

        fwrite(&width, sizeof(unsigned int),1,fptr);
        fwrite(&height, sizeof(unsigned int),1,fptr);

        height = alto;
        width = ancho;

        for (i=height; i>0; i--) {
            for (j=0;j<width;j++) {
                fputc(255, fptr);
                fputc(frameBuffer[i-1][j].r *255, fptr);
                fputc(frameBuffer[i-1][j].g *255, fptr);
                fputc(frameBuffer[i-1][j].b *255, fptr);
            }
        }
    }
    else
        printf("Error escribiendo archivo");
    fclose(fptr);

}

//Libera toda la memoria utilizada
void freeStructs(){
    int i;
    objectNode *currentO = objects->next;
    sphere *freeSphere;
    cone *freeCone;
    disc *freeDisc;
    polygon *freePoly;
    cylinder *freeCylinder;
    while(currentO){
        if(objects->tipoO == 1){
            freeSphere = (sphere *)objects->tipo;
            free(freeSphere->center);
            free(freeSphere);
        }
        free(objects->color);
        free(objects);
        objects = currentO;
        currentO = currentO->next;
    }
    if(objects){
        if(objects->tipoO == 1){
            freeSphere = (sphere *)objects->tipo;
            free(freeSphere->center);
            free(freeSphere);
        }
        else if(objects->tipoO == 2){
            freeCone = (cone *) objects->tipo;
            free(freeCone->Q);
            free(freeCone->center);
            free(freeCone);
        }
        else if(objects->tipoO == 3){
            freeDisc = (disc *) objects->tipo;
            free(freeDisc->N);
            free(freeDisc->center);
            free(freeDisc);
        }
        else if(objects->tipoO == 4){
            freeCylinder = (cylinder *) objects->tipo;
            free(freeCylinder->Q);
            free(freeCylinder->center);
            free(freeCylinder);
        }
        else if(objects->tipoO == 5){
            freePoly = (polygon *) objects->tipo;
            free(freePoly->plano->N);
            free(freePoly->vertices);
            free(freePoly->vertices2D);
            free(freePoly);
        }
        free(objects->color);
        free(objects);
    }
    lightNode *currentL = lights->next;
    while(currentL){
        free(lights->color);
        free(lights->center);
        free(lights);
        lights = currentL;
        currentL = currentL->next;
    }
    if(lights){
        free(lights->color);
        free(lights->center);
        free(lights);
    }
    for(i = 0; i < alto; i++){
        free(frameBuffer[i]);
    }
    free(frameBuffer);
}

rgb* getTexel(long double u, long double v, int d, int h, objectNode *object){
	FILE *fptr;
	rgb* texel = (rgb*) malloc(sizeof(rgb));
	int height, width, calc;
	if ((fptr = fopen(object->texFile,"r")) != NULL){


			//Determina width y height
			fread(&width,sizeof(int),1,fptr);
			width = FIX(width);

			fread(&height,sizeof(int),1,fptr);
			height = FIX(height);
			if(object->texture == 1){
				u = (long double) (d%width)/width;
				v = (long double) (h%height)/height;
			}

			u = (long double) u*width;
			v = (long double) v*height;

			calc = (v*width+u);//Calculo del pixel que se desea
			fseek(fptr, 4L*calc, SEEK_CUR);

			fgetc(fptr);//alpha
			texel->r =(double)fgetc(fptr)/255;
			texel->g =(double)fgetc(fptr)/255;
			texel->b =(double)fgetc(fptr)/255;
		}
		else{
			printf("No se encontro el archivo de imagen %c\n",object->texFile[0]);
		}
	fclose(fptr);
	return texel;
}
