//NO REVISAR EXISTENCIAS DE TABLAS PARA INSERT Y SELECT

#include "MemoryPool.h"

int main(int argc, char* archivo[])
{
	startup_memoria(); //carga las memorias


	struct parametros parametro;

	parametro.config = leer_config(archivo[1]);
	parametro.logger = iniciar_logger();

	pthread_t hilo_servidor;
	pthread_t hilo_consola;

//	int conexion = iniciar_conexion_con_FS();

//	terminar_conexion(conexion);


//	pthread_create(&hilo_servidor, NULL, servidor,  (void *) &parametro); //Falta verificacion de error en la creacion de hilos
	pthread_create(&hilo_consola, NULL, ingresar_paquete,  (void *) &parametro);

//	pthread_join(hilo_servidor,NULL);
	pthread_join(hilo_consola,NULL);

	config_destroy(parametro.config);
	log_destroy(parametro.logger);

	return 1;
}
