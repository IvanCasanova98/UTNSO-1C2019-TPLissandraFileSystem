#include "MemoryPool.h"

int main(void)
{
//	struct parametros
//	{
//		t_config* config;
//		t_log* logger;
//	};

	struct parametros parametro;

	parametro.config = leer_config();
	parametro.logger = iniciar_logger();

	pthread_t hilo_servidor;
	pthread_t hilo_consola;

	pthread_create(&hilo_servidor, NULL, servidor,  (void *) &parametro); //Falta verificacion de error en la creacion de hilos
	pthread_create(&hilo_consola, NULL, ingresar_paquete,  (void *) &parametro);

	pthread_join(hilo_servidor,NULL);
	pthread_join(hilo_consola,NULL);

	config_destroy(parametro.config);
	log_destroy(parametro.logger);

	return 1;
}
