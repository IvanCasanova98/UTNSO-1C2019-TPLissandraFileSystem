//NO REVISAR EXISTENCIAS DE TABLAS PARA INSERT Y SELECT

#include "MemoryPool.h"

int main(int argc, char* archivo[])
{
	startup_memoria(); //carga las memorias
	startup_semaforos();


	struct parametros parametro;

	parametro.config = leer_config(archivo[1]);
	parametro.logger = iniciar_logger();

	pthread_t hilo_servidor;
	pthread_t hilo_consola;
	pthread_t hilo_journal;

	//HANDSHAKE CON EL FS.
	int max_value_FS = recibir_max_value_del_fs(parametro.config,parametro.logger);
	log_info(parametro.logger,"MAX VALUE RECIBIDO: %d",max_value_FS);

	pthread_create(&hilo_servidor, NULL, servidor,  (void *) &parametro); //Falta verificacion de error en la creacion de hilos
	pthread_create(&hilo_consola, NULL, ingresar_paquete,  (void *) &parametro);
	pthread_create(&hilo_journal, NULL, time_journal,  (void *) &parametro);

	pthread_join(hilo_servidor,NULL);
	pthread_join(hilo_consola,NULL);
	pthread_join(hilo_journal,NULL);

	config_destroy(parametro.config);
	log_destroy(parametro.logger);

	return 1;
}
