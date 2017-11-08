
#include "funciones.c"

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

	tIteracionSalida--;

	if(NtamanioGrilla <10)
	{
		printf("El tamano de la grilla debe ser mayor a 10\n");
		return 1;
	}
	if(HNumeroHebras<=0)
	{
		printf("La cantidad de hebras debe ser mayor a 0\n");
		return 1;
	}
	if(TnumeroPasos<= 0 )
	{
		printf("El numero de pasos debe ser mayor a 0\n ");
		return 1;
	}
	if(fNombreArchivoSalida == NULL)
	{
		printf("Falta el nombre del archivo de salida\n");
		return 1;
	}
	if(tIteracionSalida < 0)
	{
		printf("La iteracion de salida no debe ser menor a 0\n");
		return 1;
	}

	int i, t;
	void * ptr = NULL;

	matriz = crearBaseMatrizSinHebras(NtamanioGrilla,TnumeroPasos);
	
	hebra** hebras = (hebra**)calloc(HNumeroHebras,sizeof(hebra*));
	hebras = asignarVariables(NtamanioGrilla,hebras,HNumeroHebras);

	threads = (pthread_t*) calloc(HNumeroHebras, sizeof(pthread_t));

	for(t=1;t<TnumeroPasos;t++)
	{
		for(i=0; i<HNumeroHebras; i++)
		{	
			pthread_join(threads[i], &ptr);
		}
		
		for(i=0; i<HNumeroHebras; i++)
		{
			hebras[i]->t=t;
			pthread_create(&threads[i], NULL,rellenar,(void*)hebras[i]);
		}
	}

	crearSalida(matriz, NtamanioGrilla, tIteracionSalida);

	return 0;
}