#include "Lissandra.h"


t_dictionary* TablasCompactacion=NULL;
t_dictionary* TablasSem=NULL;
t_dictionary* memTable=NULL;
t_bitarray* bitmap;
t_list* ListaSem=NULL;
sem_t SemaforoMemtable;
sem_t SemaforobitArray;
bool estaDump =0;

int main(void)
{
	logger = iniciar_logger();
	sem_init(&SemaforoMemtable,0,1);
	sem_init(&SemaforobitArray,0,1);
	t_config* config = leer_config();

	if(hayMetadata()){
		printf("%s\nCreando Metadata...\n",CYAN);
		crearMetadataDePuntoDeMontaje();
		printf("Metadata Creada con exito.\n%s",NORMAL_COLOR);
	}

	if(hayBitmap()){
		printf("%s\nCreando Bitmap...\n",CYAN);
		crearArchivoBitmap();
		printf("Bitmap Creado con exito.\n%s",NORMAL_COLOR);
	}

	crearBitMap();
	pthread_t consola,dumpeo,dump,compactar,hiloListener;


	//int tiempoEntreDump = atoi(config_get_string_value(config, "TIEMPO_DUMP"));
	//gettimeofday(&tiempoHastaDump,NULL);


	levantarHilosTablasExistentesCompactacion();
	pthread_create(&dump,NULL,chekearDumpeo,NULL);
	pthread_create(&consola,NULL,prenderConsola,NULL);
	pthread_create(&hiloListener,NULL,hiloEscucha,NULL);


	config_destroy(config);

	log_info(logger, "FIN CONEXION");
	//log_destroy(logger);
	pthread_join(hiloListener, (void**)NULL);
	pthread_join(dump, (void**)NULL);
	pthread_join(consola, (void**)NULL);
	return EXIT_SUCCESS;
}
