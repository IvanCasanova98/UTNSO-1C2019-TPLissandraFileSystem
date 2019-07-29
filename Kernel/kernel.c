//VER QUE PASA CON LOS DESCRIBE GLOBAL COMO REQUEST SIMPLE

//REVISAR BYTES PARA LOS MALLOCS CUANDO EJECUTAS LA FUNCION SERIALIZAR, ESTRUCTURA REPETIDA
//ENVIAR TAMANIO COMPLETO DEL PAQUETE, Y LUEGO BUFFER
//REVISAR EXISTENCIA DE TABLA ANTES DE ENVIAR INSERT O SELECT

//DROP

#include "kernel.h"


int main(void)
{
	startup_diccionario();
	startup_cola_ready();
	startup_lista_seeds();

	struct parametros parametro;

	parametro.config = leer_config();
	parametro.logger = iniciar_logger();

	levantar_kernel(parametro.config, parametro.logger);

	pthread_t hilo_consola;
	pthread_t hilo_planificador;

	pthread_create(&hilo_consola, NULL, consola, NULL);
	pthread_create(&hilo_planificador, NULL, planificador, (void *) &parametro);

	pthread_join(hilo_consola,NULL);
	pthread_join(hilo_planificador,NULL);

	terminar_kernel(parametro.logger,parametro.config);

	return 1;
}

void levantar_kernel(t_config* config, t_log* logger)
{
	char* ip = config_get_string_value(config, "IP_MEMORIA");
	char* puerto = config_get_string_value(config, "PUERTO_MEMORIA");

	int conexion = iniciar_conexion_inicial(logger, ip, puerto);

	pedir_seed(conexion);
	mostrar_lista_seeds();

	close(conexion);
}

