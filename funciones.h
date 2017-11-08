#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#define Ccuadrado 1.0
#define DtDdcuadrado 0.0025

typedef struct hebra
{
	int id;
	int t;
	int X;
	int Y;
	int tamX;
	int tamY;
	
}hebra;

//Globales
float*** matriz;
pthread_t* threads;

int NtamanioGrilla = 0;
int TnumeroPasos = 0;
int HNumeroHebras = 0;
char * fNombreArchivoSalida;
int tIteracionSalida = 0;

/*
	Imprime la salida en la iteración t. Si t>T entonces la salida no se imprime.
	Entrada: puntero de matriz, tamaño de la grilla, número de iteración en la cual se imprime la salida.
	Salida: nada.
*/
void imprimirSalida(float*** matriz, int N, int t);

/*
	Imprime la traza de todas las iteraciones.
	Entrada: puntero de matriz, tamaño de la grilla, cantidad de iteraciones a realizar.
	Salida: nada.
*/
void imprimirTraza(float*** matriz, int N, int T);

/*
	Crea una matriz (dos dimensiones) con una dimensión más para realizar las iteraciones.
	Entrada: tamaño de la grilla, cantidad total de iteraciones a realizar.
	Salida: puntero a la matriz de números flotantes.
*/
float *** crearBaseMatrizSinHebras(int N, int t);

/*
	Imprime el contenido de una hebra.
	Entrada: hebra a imprimir.
	Salida: nada.
*/
void imprimirHebra(hebra * thread);

/*
	Se encarga de hacer la ecuación de Schroendinger en la matriz global.
	Entrada: hebra que trabaja en una sección determinada de la matriz.
	Salida: nada.

*/
void* rellenar(void* thread);

/*
	Asigna los valores a una hebra en su estructura respectiva.
	Entrada: tamaño de la grilla, puntero hacia las estructuras de las hebras, cantidad total de hebras.
	Salida: puntero con estructuras de hebras con los valores asignados.
*/
hebra** asignarVariables(int N, hebra** hebras, int cantH);

/*
	Crea un archivo de salida con la iteración de salida t. Si t>T, entonces la salida no se crea.
	Entrada: puntero de la matriz, tamaño de la matriz, iteración de salida.
	Salida: nada.
*/
void crearSalida(float*** matriz, int N, int t);