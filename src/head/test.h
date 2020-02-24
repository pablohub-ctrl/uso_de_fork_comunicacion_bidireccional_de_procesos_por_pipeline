#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <png.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdbool.h>


int loadImagen(int* dimensiones,int** matriz,int fichero);
float pixelmodificado(int ancho, int alto, int** matriz, float** nuevaMatriz, int** pasabajo);
void rectificar(float** nuevamatriz1, int* dimensiones);
void liberarMatriz(float** matriz, int alto);
float clasificacion(float** matriz, int* dimensiones);
int clasifica(float n, float umbral, int porcentaje);
int convolucion(int** matriz, int* anchoxalto, float** nuevaMatriz, int** pasabajo);
char* nombre_imagen(int n);
char* nombre_imagen_salida(int n);
char* nombre_imagen_printf(int n);
int** leerFichero(char* nombre);
float valormasAlto(float a, float b, float c);
float valorhighCuadrante(float** matrizrectificada, int i, int j);
void pooling(float** matrizrectificada, float** matrizpooling, int* dimensionPooling);
void rectificar(float** nuevamatriz1, int* dimensiones);
