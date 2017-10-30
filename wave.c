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

/*
typedef struct hebraDos
{
	int id;
	int iteracion;
	int filaInicial;
	int filaFinal;
	
} hebraDos;
*/

//Globales
float*** matriz;
pthread_t* threads;

//hebraDos** hebrasDos;

pthread_mutex_t* locks;

int NtamanioGrilla = 0;
int TnumeroPasos = 0;
int HNumeroHebras = 0;
char * fNombreArchivoSalida;
int tIteracionSalida = 0;

void imprimirSalida(float*** matriz, int N, int t)
{
	//printf("Inicio imprimirSalida\n");
	printf("Salida: \n");
	int i, j;
	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; j++)
		{
			printf("%.10f ", matriz[i][j][t]);
		}
		printf("\n");
	}

	//printf("Fin imprimirSalida\n");
}

void imprimirTraza(float*** matriz, int N, int T)
{
	//printf("Inicio imprimirTraza\n");

	int i, j, k;
	for(k=0; k<T; k++)
	{
		printf("> Iteracion #%d\n", k);
		for (i=0; i<N; ++i)
			{
				for (j=0; j<N; ++j)
				{
					printf("%.10f ", matriz[i][j][k]);
				}
				printf("\n");
			}
		printf("\n");
	}
	//printf("Fin imprimirTraza\n");
}
/*
void imprimirHebraDos(hebraDos* heb)
{
	printf("ID: %d, Inicio: %d, Fin: %d\n", heb->id, heb->filaInicial, heb->filaFinal);
}
*/
float *** crearBaseMatrizSinHebras(int N, int t)
{
	//printf("Inicio crearBaseMatrizSinHebras\n");
	float *** matriz = (float ***)calloc(N,sizeof(float**));
	int i,j;
	for (i=0; i<N; ++i)
	{
		matriz[i] = (float **)calloc(N,sizeof(float*));
		for(j=0; j<N; j++)
		{
			matriz[i][j] = (float *)calloc(t,sizeof(float));
		}
	}
	
	float valueA,valueB;
	valueA = N*0.4;
	valueB = N*0.6;
	i = (int)valueA;
	i = (int)valueB;

	//printf("> Valor A:%d, Valor B:%d\n", (int)valueA, (int)valueB);

	int aux=(i-valueA<0)?1:0;

	for(i=(int)valueA+aux;i<(int)valueB;i++)
	{
		for(j=(int)valueA+aux;j<(int)valueB;j++)
		{
			printf("%d %d\n",i,j );
			matriz[i][j][0]=20;
		}
	}
	
	//printf("Fin crearBaseMatrizSinHebras\n");
	return matriz;

}


void imprimirHebra(hebra * thread)
{
	//printf("Inicio imprimirHebra\n");
	printf("id %d\nX=%d\nY=%d\ntamX=%d\ntamY=%d\nt=%d\n\n",thread->id,thread->X,thread->Y,thread->tamX,thread->tamY , thread->t );
	//printf("Fin imprimirHebra\n");
}



void* rellenar(void* thread)
{
	//printf("Inicio rellenar\n");
	hebra* laHebra = (hebra*) thread;

	imprimirHebra(laHebra);

	int k = laHebra->t;
	int i, j;
	int p, q;
	for (i =laHebra->X, p=0; p<laHebra->tamX; ++i,p++)
	{
		for (j = laHebra->Y, q=0; q<laHebra->tamY; ++j, q++)
		{
			if(i==0 || j==0 || i==NtamanioGrilla-1 || j==NtamanioGrilla-1)
			{
				matriz[i][j][k] = 0;
			}
			else if(k==1)
			{	
				matriz[i][j][k] = matriz[i][j][0] + ((Ccuadrado*DtDdcuadrado)/2)*(matriz[i+1][j][0] + matriz[i-1][j][0]+ matriz[i][j+1][0] + matriz[i][j-1][0]);
			}
			else
			{
				matriz[i][j][k] = matriz[i][j][k-1]-matriz[i][j][k-2] + (Ccuadrado*DtDdcuadrado)*(matriz[i+1][j][k-1] + matriz[i-1][j][k-1]+ matriz[i][j+1][k-1] + matriz[i][j-1][k-1] - 4*matriz[i][j][k-1]);
			}
		}
	}
	void * a = NULL;
	//printf("Fin rellenar\n");
	return a;
}

/*
void* rellenarPorHebra(void* h)
{
	int idHebra = (int) h;

	imprimirHebraDos(hebrasDos[idHebra]);

	int i, j;

	for(i = hebrasDos[idHebra]->filaInicial; i<hebrasDos[idHebra]->filaFinal; i++)
	{
		for(j=0; j<NtamanioGrilla; j++)
		{
			if(i==0 || j==0 || i==NtamanioGrilla-1 || j==NtamanioGrilla-1)
			{
				matriz[i][j][hebrasDos[idHebra]->iteracion] = 0;
			}
			else if(hebrasDos[idHebra]->iteracion==1)
			{	
				matriz[i][j][hebrasDos[idHebra]->iteracion] = matriz[i][j][0] + ((Ccuadrado*DtDdcuadrado)/2)*(matriz[i+1][j][0] + matriz[i-1][j][0]+ matriz[i][j+1][0] + matriz[i][j-1][0]);
			}
			else
			{
				matriz[i][j][hebrasDos[idHebra]->iteracion] = matriz[i][j][hebrasDos[idHebra]->iteracion-1]-matriz[i][j][hebrasDos[idHebra]->iteracion-2] + (Ccuadrado*DtDdcuadrado)*(matriz[i+1][j][hebrasDos[idHebra]->iteracion-1] + matriz[i-1][j][hebrasDos[idHebra]->iteracion-1]+ matriz[i][j+1][hebrasDos[idHebra]->iteracion-1] + matriz[i][j-1][hebrasDos[idHebra]->iteracion-1] - 4*matriz[i][j][hebrasDos[idHebra]->iteracion-1]);
			}
		}
	}

}
*/

hebra** asignarVariables(int N, hebra** hebras, int cantH)
{
	//printf("Inicio asignarVariables\n");
	if(cantH>N)
	{
		//printf("> Hebras mayor que N\n");
		return hebras;
	}
	if(cantH==0)
	{
		return hebras;
	}
	//printf("Cantidad hebras: %d\n", cantH );
	int i;
	int div = N/cantH;
	int resto = N%cantH;
	int posicion = 0;
	
	for (i=0; i<cantH; ++i)
	{
		hebras[i] = calloc(1,sizeof(hebra));
		hebras[i]->id = i;
		int aux = (i<resto)? 1 : 0;
		hebras[i]->tamX = N;
		hebras[i]->tamY = div+aux;
		hebras[i]->X = 0;
		hebras[i]->Y = posicion;
		posicion = div+aux+posicion;
	}
	//printf("Fin asignarVariables\n");
	return hebras;
}

/*
void asignarValoresHebra(int N, hebraDos** hebras, int H)
{
	int i;
	int div = N/H;
	int resto = N%H;

	for(i=0; i<H-1; i++)
	{
		hebras[i] = calloc(1, sizeof(hebraDos));
		hebras[i]->id = i;
		hebras[i]->filaInicial = div*i;
		hebras[i]->filaFinal = div*(i+1);
	}
	//printf(">>>>> I: %d\n", i);
	
	hebras[H-1] = calloc(1, sizeof(hebraDos));
	hebras[i]->id = i;
	hebras[i]->filaInicial = div*i;
	hebras[i]->filaFinal = div*(i+1)+resto;
}
*/

void crearSalida(float*** matriz, int N, int t)
{
	//printf("Inicio crearSalida\n");

	FILE* archivoSalida;
	archivoSalida = fopen(fNombreArchivoSalida, "w");
	
	int i, j;
	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; j++)
		{
			fprintf(archivoSalida, "%.10f ", matriz[i][j][t]);
		}
		fprintf(archivoSalida, "\n");
	}
	fclose(archivoSalida);

	//printf("Fin crearSalida\n");
}


int main(int argc, char **argv)
{
	int c;
	/*if(argc!=10){
		if(argc>10){
			printf("\nERROR: TIENE MAS PARAMETOS DE ENTRADA DE LOS REQUERIDOS\n");
		}
		else{
			printf("\nERROR: TIENE MENOS PARAMETOS DE ENTRADA DE LOS REQUERIDOS\n");
		}
		return 1;
	}*/
	/*
	Si el arreglo con las opciones "abc:d:e::", después de cada
	caracter lleva...
	: -> significa que esa opción requiere un argumento.
	:: -> significa que esa opción puede recibir un argumento (opcional).
	   -> si no lleva nada, significa que esa opción es opcional y puede ser omitida.
	*/
	//Variables donde se almancenan los parametros obtenidos por getopt
	
	while ((c = getopt (argc, argv, "N:T:H:f:t:")) != -1)
	{
		switch(c)
		{
			case 'N':
			sscanf(optarg, "%d", &NtamanioGrilla);
			break;

			case 'T':
			sscanf(optarg, "%d", &TnumeroPasos);
			break;

			case 'H':
			sscanf(optarg, "%d", &HNumeroHebras);
			break;

			case 'f':
			fNombreArchivoSalida = optarg;
			break;

			case 't':
			sscanf(optarg, "%d", &tIteracionSalida);
			break;
			case '?':
        	if (optopt == 'i' || optopt == 'h' || optopt == 'c' || optopt == 'n' || optopt == 'm' || optopt == 's')
          		{
          			fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
          		}
        	else if (isprint (optopt))
          		{
          			fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
          		}
        	else
          		{
          			fprintf (stderr,
                   "Opcion con caracter desconocido `\\x%x'.\n",
                   optopt);
          		}
        	return 1;

        	default:
        	abort();
		}

	}

	int i, t;
	void * ptr = NULL;
	matriz = crearBaseMatrizSinHebras(NtamanioGrilla,TnumeroPasos);
	
	hebra** hebras = (hebra**)calloc(HNumeroHebras,sizeof(hebra*));
	hebras = asignarVariables(NtamanioGrilla,hebras,HNumeroHebras);

	//hebrasDos = (hebraDos**) calloc(HNumeroHebras, sizeof(hebraDos*));
	//asignarValoresHebra(NtamanioGrilla, hebrasDos, HNumeroHebras);

	threads = (pthread_t*) calloc(HNumeroHebras, sizeof(pthread_t));

	/*
	int k, h;

	for(k=0; k<TnumeroPasos; k++)
	{
		for(h=0; h<HNumeroHebras; h++)
		{
			hebrasDos[h]->iteracion = k;
			pthread_create(&threads[h], NULL, rellenarPorHebra, (void*)h);
		}
	}
	*/
	
	for(t=1;t<TnumeroPasos;t++)
	{
		for(i=0; i<HNumeroHebras; i++)
		{
			pthread_join(threads[i], &ptr);
			hebras[i]->t=t;
			pthread_create(&threads[i], NULL,rellenar,(void*)hebras[i]);
		}
	}
	
	//Retorno del join
	for(i=0; i<HNumeroHebras; i++)
	{	
		pthread_join(threads[i], &ptr);
	}
	

	imprimirTraza(matriz, NtamanioGrilla, TnumeroPasos);
	imprimirSalida(matriz, NtamanioGrilla, tIteracionSalida);

	crearSalida(matriz, NtamanioGrilla, tIteracionSalida);

	return 0;
}