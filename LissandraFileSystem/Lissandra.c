#include "Lissandra.h"


t_dictionary* TablasCompactacion=NULL;
t_dictionary* TablasSem=NULL;
t_dictionary* memTable=NULL;
t_bitarray* bitmap;
t_list* ListaSem=NULL;
sem_t SemaforoMemtable;
bool estaDump =0;

int main(void)
{
	logger = iniciar_logger();
	sem_init(&SemaforoMemtable,0,1);
	t_config* config = leer_config();
	if(hayMetadata() == 0) crearMetadataDePuntoDeMontaje();
	if(hayBitmap()   == 0) crearArchivoBitmap();

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
