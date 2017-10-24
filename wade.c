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

pthread_mutex_t* locks;

int NtamanioGrilla=0;
int TnumeroPasos=0;
int HNumeroHebras=0;
char * fNombreArchivoSalida;
int tIteracionSalida=0;

void imprimir(float*** matriz,int N,int t){
	printf("SALI DEL PRIMER FOR\n\n");
	int i,j,k;
	for(k=0;k<t;k++){
		for (i = 0; i < N; ++i)
			{
				for (j = 0; j < N; ++j)
				{
					printf("%.10f ", matriz[i][j][k]);
				}
				printf("\n");
			}
		printf("\n");
	}
	
}

float *** crearBaseMatrizSinHebras(int N, int t){
	float *** matriz=(float ***)calloc(N,sizeof(float**));
	int i,j;
	for (i = 0; i < N; ++i){
		matriz[i]=(float **)calloc(N,sizeof(float*));
		for(j=0;j<N;j++){
			matriz[i][j]=(float *)calloc(t,sizeof(float));
		}
	}

	float valueA,valueB;
	valueA=N*0.4;
	valueB=N*0.6;
	i=(int)valueA;
	i=(int)valueB;
	int aux=(i-valueA<0)?1:0;
	for(i=(int)valueA+aux;i<(int)valueB;i++){
		for(j=(int)valueA+aux;j<(int)valueB;j++){
			printf("%d %d\n",i,j );
			matriz[i][j][0]=20;
		}
	}
	printf("return\n");
	//imprimir(matriz,N,t);
	return matriz;

}

float ***rellenarSinHebras(float *** matriz,int N,int t){
	//para i=0 o i=n-1 o j=0 o j=N-1 esto es 0
	// para t=1
	// matriz[i][j][1]=matriz[i][j][0] + valor(matriz[i+1][j][0] + matriz[i-1][j][0]+
	// matriz[i][j+1][0] + matriz[i][j-1][0])

	//otros
	// matriz[i][j][t]=matriz[i][j][t-1]-matriz[i][j][t-2] + valor(matriz[i+1][j][0] 
	//+ matriz[i-1][j][0]+ matriz[i][j+1][0] + matriz[i][j-1][0] - 4*matriz[i][j][t-1])

	int i,j,k;
	for (k = 1; k < t; ++k){
		for (i = 0; i < N; ++i){
			for (j = 0; j < N; ++j){
				if(i==0 || j==0 || i==N-1 || j==N-1){
					matriz[i][j][k]=0;
				}
				else if(k==1){
					//printf("matriz[%d][%d][%d]\n",i,j,k);
					//printf("matriz[%d][%d][%d] = %f\n",i,j,k-1, matriz[i][j][0]);
					//printf("matriz[%d][%d][%d] = %f\n",i+1,j,k-1, matriz[i+1][j][0]);
					//printf("matriz[%d][%d][%d] = %f\n",i-1,j,k-1, matriz[i-1][j][0]);
					//printf("matriz[%d][%d][%d] = %f\n",i,j+1,k-1, matriz[i][j+1][0]);
					//printf("matriz[%d][%d][%d] = %f\n",i,j-1,k-1, matriz[i][j-1][0]);
					
					//printf("%d %d %d %f\n\n",i,j,k,valorA );
					matriz[i][j][k]=matriz[i][j][0] + ((Ccuadrado*DtDdcuadrado)/2)*(matriz[i+1][j][0] + matriz[i-1][j][0]+ matriz[i][j+1][0] + matriz[i][j-1][0]);
					//printf("matriz[%d][%d][%d] = %f + (%f)*(%f + %f + %f + %f) = %f\n",i,j,k,matriz[i][j][0],(Ccuadrado*DtDdcuadrado)/2,matriz[i+1][j][0],matriz[i-1][j][0],matriz[i][j+1][0],matriz[i][j-1][0],matriz[i][j][k]);
				}
				else{
					//printf("%d %d %d %f\n\n",i,j,k,valorB);
					//printf("matriz[%d][%d][%d]\n",i,j,k);
					//printf("matriz[%d][%d][%d] = %f\n",i,j,k-1, matriz[i][j][k-1]);
					//printf("matriz[%d][%d][%d] = %f\n",i,j,k-2, matriz[i][j][k-2]);
					//printf("matriz[%d][%d][%d] = %f\n",i+1,j,k-1, matriz[i+1][j][k-1]);
					//printf("matriz[%d][%d][%d] = %f\n",i-1,j,k-1, matriz[i-1][j][k-1]);
					//printf("matriz[%d][%d][%d] = %f\n",i,j+1,k-1, matriz[i][j+1][k-1]);
					//printf("matriz[%d][%d][%d] = %f\n",i,j-1,k-1, matriz[i][j-1][k-1]);
					//printf("matriz[%d][%d][%d] = %f\n",i,j-1,k-1, 4*matriz[i][j][k-1]);
					matriz[i][j][k]=matriz[i][j][k-1]-matriz[i][j][k-2] + (Ccuadrado*DtDdcuadrado)*(matriz[i+1][j][k-1] + matriz[i-1][j][k-1]+ matriz[i][j+1][k-1] + matriz[i][j-1][k-1] - 4*matriz[i][j][k-1]);
					//printf("matriz[%d][%d][%d] = %f - %f + (%f)*(%f + %f + %f + %f - %f) = %f\n",i,j,k,matriz[i][j][k-1],matriz[i][j][k-2],(Ccuadrado*DtDdcuadrado),matriz[i+1][j][k-1],matriz[i-1][j][k-1],matriz[i][j+1][k-1],matriz[i][j-1][k-1],4*matriz[i][j][k-1],matriz[i][j][k]);	
				}
			}
		}
	}

	return matriz;
}
void imprimirHebra(hebra * thread){
	printf("id %d\nX=%d\nY=%d\ntamX=%d\ntamY=%d\nt=%d\n\n",thread->id,thread->X,thread->Y,thread->tamX,thread->tamY , thread->t );

}


void* rellenar(void* thread){
	hebra* laHebra=(hebra*) thread;
	int k=laHebra->t;
	int i,j;
	//imprimirHebra(thread);
	int p,q;
	for (i =laHebra->X,p=0; p <laHebra->tamX ; ++i,p++){
		for (j = laHebra->Y,q=0; q < laHebra->tamY; ++j,q++){
			if(i==0 || j==0 || i==NtamanioGrilla-1 || j==NtamanioGrilla-1){
				//printf("matriz[%d][%d][%d] = %f\n",i,j,k,matriz[i][j][k]);
				matriz[i][j][k]=0;
			}
			else if(k==1){
				//printf("matriz[%d][%d][%d]\n",i,j,k);
				//printf("matriz[%d][%d][%d] = %f\n",i,j,k-1, matriz[i][j][0]);
				//printf("matriz[%d][%d][%d] = %f\n",i+1,j,k-1, matriz[i+1][j][0]);
				//printf("matriz[%d][%d][%d] = %f\n",i-1,j,k-1, matriz[i-1][j][0]);
				//printf("matriz[%d][%d][%d] = %f\n",i,j+1,k-1, matriz[i][j+1][0]);
				//printf("matriz[%d][%d][%d] = %f\n",i,j-1,k-1, matriz[i][j-1][0]);
				
				matriz[i][j][k]=matriz[i][j][0] + ((Ccuadrado*DtDdcuadrado)/2)*(matriz[i+1][j][0] + matriz[i-1][j][0]+ matriz[i][j+1][0] + matriz[i][j-1][0]);
				//printf("hebra=%d matriz[%d][%d][%d] = %f + (%f)*(%f + %f + %f + %f) = %f\n",laHebra->id,i,j,k,matriz[i][j][0],(Ccuadrado*DtDdcuadrado)/2,matriz[i+1][j][0],matriz[i-1][j][0],matriz[i][j+1][0],matriz[i][j-1][0],matriz[i][j][k]);
			}
			else{
				//printf("matriz[%d][%d][%d]\n",i,j,k);
				//printf("matriz[%d][%d][%d] = %f\n",i,j,k-1, matriz[i][j][k-1]);
				//printf("matriz[%d][%d][%d] = %f\n",i,j,k-2, matriz[i][j][k-2]);
				//printf("matriz[%d][%d][%d] = %f\n",i+1,j,k-1, matriz[i+1][j][k-1]);
				//printf("matriz[%d][%d][%d] = %f\n",i-1,j,k-1, matriz[i-1][j][k-1]);
				//printf("matriz[%d][%d][%d] = %f\n",i,j+1,k-1, matriz[i][j+1][k-1]);
				//printf("matriz[%d][%d][%d] = %f\n",i,j-1,k-1, matriz[i][j-1][k-1]);
				//printf("matriz[%d][%d][%d] = %f\n",i,j-1,k-1, 4*matriz[i][j][k-1]);
				matriz[i][j][k]=matriz[i][j][k-1]-matriz[i][j][k-2] + (Ccuadrado*DtDdcuadrado)*(matriz[i+1][j][k-1] + matriz[i-1][j][k-1]+ matriz[i][j+1][k-1] + matriz[i][j-1][k-1] - 4*matriz[i][j][k-1]);
				//printf("hebra=%d matriz[%d][%d][%d] = %f - %f + (%f)*(%f + %f + %f + %f - %f) = %f\n",laHebra->id,i,j,k,matriz[i][j][k-1],matriz[i][j][k-2],(Ccuadrado*DtDdcuadrado),matriz[i+1][j][k-1],matriz[i-1][j][k-1],matriz[i][j+1][k-1],matriz[i][j-1][k-1],4*matriz[i][j][k-1],matriz[i][j][k]);	
			}
		}
	}
	void * a=NULL;
	return a;
}

hebra** asignarVariables(int N,hebra** hebras, int cantH){
	if(cantH>N){
		return hebras;
	}
	if(cantH==0){
		return hebras;
	}
	printf("%d\n",cantH );
	int i;
	int div=N/cantH;
	int resto=N%cantH;
	int posicion=0;
	
	for (i = 0; i < cantH; ++i){	
		hebras[i]=calloc(1,sizeof(hebra));
		hebras[i]->id=i;
		int aux = (i<resto)? 1 : 0;
		hebras[i]->tamX=N;
		hebras[i]->tamY=div+aux;
		hebras[i]->X=0;
		hebras[i]->Y=posicion;
		posicion=div+aux+posicion;
	}
	return hebras;
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

	int i,t;
	void * ptr = NULL;
	matriz=crearBaseMatrizSinHebras(NtamanioGrilla,tIteracionSalida);
	//imprimir(matriz,NtamanioGrilla,tIteracionSalida);
	hebra** hebras=(hebra**)calloc(HNumeroHebras,sizeof(hebra*));
	hebras=asignarVariables(NtamanioGrilla,hebras,HNumeroHebras);
	threads=(pthread_t*) calloc(HNumeroHebras, sizeof(pthread_t));
	for(t=1;t<TnumeroPasos;t++){

		
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

	//matriz=rellenarSinHebras(matriz,NtamanioGrilla,tIteracionSalida);
	imprimir(matriz,NtamanioGrilla,tIteracionSalida);


	return 0;
}