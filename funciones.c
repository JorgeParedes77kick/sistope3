#include "funciones.h"

void imprimirSalida(float*** matriz, int N, int t)
{
	if(tIteracionSalida >= TnumeroPasos)
	{
		printf("No se puede imprimir la salida porque t es mayor a T\n");
		return;
	}

	printf("Salida: \n");
	int i, j;
	for(i=0; i<N; ++i)
	{
		for(j=0; j<N; j++)
		{
			printf("%.10f ", matriz[t][i][j]);
		}
		printf("\n");
	}
}

void imprimirTraza(float*** matriz, int N, int T)
{
	int i, j, k;
	for(k=0; k<T; k++)
	{
		printf("> Iteracion #%d\n", k);
		for (i=0; i<N; ++i)
			{
				for (j=0; j<N; ++j)
				{
					printf("%.10f ", matriz[k][i][j]);
				}
				printf("\n");
			}
		printf("\n");
	}
}

float *** crearBaseMatrizSinHebras(int N, int t)
{
	float *** matriz = (float ***)calloc(t,sizeof(float**));
	int i,j;
	for (i=0; i<t; ++i)
	{
		matriz[i] = (float **)calloc(N,sizeof(float*));
		for(j=0; j<N; j++)
		{
			matriz[i][j] = (float *)calloc(N,sizeof(float));
		}
	}

	float valueA,valueB;
	valueA = N*0.4;
	valueB = N*0.6;
	i = (int)valueA;
	i = (int)valueB;

	int aux=(i-valueA<0)?1:0;

	for(i=(int)valueA+aux;i<(int)valueB;i++)
	{
		for(j=(int)valueA+aux;j<(int)valueB;j++)
		{
			matriz[0][i][j]=20;
		}
	}
	return matriz;
}

void imprimirHebra(hebra * thread)
{
	printf("id %d\nX=%d\nY=%d\ntamX=%d\ntamY=%d\nt=%d\n\n",thread->id,thread->X,thread->Y,thread->tamX,thread->tamY , thread->t );
}

void* rellenar(void* thread)
{
	hebra* laHebra = (hebra*) thread;

	int k = laHebra->t;
	int i, j;
	int p, q;

	for (i =laHebra->X, p=0; p<laHebra->tamX; ++i,p++)
	{
		for (j = laHebra->Y, q=0; q<laHebra->tamY; ++j, q++)
		{
			if(i==0 || j==0 || i==NtamanioGrilla-1 || j==NtamanioGrilla-1)
			{
				matriz[k][i][j] = 0;
			}
			else if(k==1)
			{	
				matriz[k][i][j] = matriz[0][i][j] + ((Ccuadrado*DtDdcuadrado)/2)*(matriz[0][i+1][j] + matriz[0][i-1][j]+ matriz[0][i][j+1] + matriz[0][i][j-1]);
			}
			else
			{
				matriz[k][i][j] = matriz[k-1][i][j]-matriz[k-2][i][j] + (Ccuadrado*DtDdcuadrado)*(matriz[k-1][i+1][j] + matriz[k-1][i-1][j]+ matriz[k-1][i][j+1] + matriz[k-1][i][j-1] - 4*matriz[k-1][i][j]);
			}
		}
	}

	pthread_exit(NULL);
}

hebra** asignarVariables(int N, hebra** hebras, int cantH)
{
	if(cantH>N)
	{
		return hebras;
	}
	if(cantH==0)
	{
		return hebras;
	}
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
	return hebras;
}

void crearSalida(float*** matriz, int N, int t)
{
	if(tIteracionSalida >= TnumeroPasos)
	{
		printf("No se puede imprimir la salida porque t es mayor a T\n");
		return;
	}

	FILE* archivoSalida;
	archivoSalida = fopen(fNombreArchivoSalida, "wb");
	
	int i;
	for(i=0; i<N; ++i)
	{
		fwrite(matriz[t][i],N,sizeof(float),archivoSalida);
	}
	fclose(archivoSalida);
}