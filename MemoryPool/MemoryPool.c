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

	pthread_create(&hilo_servidor, NULL, servidor,  (void *) &parametro); //Falta verificacion de error en la creacion de hilos
	pthread_create(&hilo_consola, NULL, ingresar_paquete,  (void *) &parametro);


	char** IP_SEEDS = config_get_array_value(parametro.config, "IP_SEEDS");
	char** PUERTO_SEEDS = config_get_array_value(parametro.config, "PUERTO_SEEDS");

//	uint32_t tamanioDeUnoIP= strlen(IP_SEEDS[0]);
//	printf("\n\n\n DE UN IP: %d\n\n\n",tamanioDeUnoIP);
//
//	uint32_t tamanioARRAYIP = strlen(IP_SEEDS);
//	printf("\n\n\n DEL ARRAY IP: %d\n\n\n",tamanioARRAYIP);
//
//	uint32_t tamanioDeUnoPUERTO= strlen(PUERTO_SEEDS[0]);
//	printf("\n\n\n DE UN PUERTO: %d\n\n\n",tamanioDeUnoPUERTO);
//
//	uint32_t tamanioARRAYPUERTO = strlen(PUERTO_SEEDS);
//	printf("\n\n\n DEL ARRAY PUERTO: %d\n\n\n",tamanioARRAYPUERTO);



	pthread_join(hilo_servidor,NULL);
	pthread_join(hilo_consola,NULL);

	config_destroy(parametro.config);
	log_destroy(parametro.logger);

	return 1;
}
