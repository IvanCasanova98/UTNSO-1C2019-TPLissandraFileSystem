//VER QUE PASA CON LOS DESCRIBE GLOBAL COMO REQUEST SIMPLE

//REVISAR BYTES PARA LOS MALLOCS CUANDO EJECUTAS LA FUNCION SERIALIZAR, ESTRUCTURA REPETIDA
//ENVIAR TAMANIO COMPLETO DEL PAQUETE, Y LUEGO BUFFER
//REVISAR EXISTENCIA DE TABLA ANTES DE ENVIAR INSERT O SELECT

#include "kernel.h"


int main(void){

	startup_diccionario();
	startup_cola_ready();
	startup_lista_seeds();

	struct parametros parametro;

	parametro.config = leer_config();
	parametro.logger = iniciar_logger();

	char* ip = config_get_string_value(parametro.config, "IP_MEMORIA");
	char* puerto = config_get_string_value(parametro.config, "PUERTO_MEMORIA");

	int conexion = iniciar_conexion(parametro.logger, ip, puerto);
	//parametro.conexion = conexion;

	pedir_seed(conexion);
	mostrar_lista_seeds();


	close(conexion);
//	SEED memoria = elegir_memoria();

//	conexion = iniciar_conexion(logger, memoria.IP, memoria.PUERTO);//conectar a la memoria elegida


	pthread_t hilo_consola;
	pthread_t hilo_planificador;

	pthread_create(&hilo_consola, NULL, consola, NULL);
	pthread_create(&hilo_planificador, NULL, planificador, (void *) &parametro);

	pthread_join(hilo_consola,NULL);
	pthread_join(hilo_planificador,NULL);


	terminar_kernel(parametro.logger,parametro.config,conexion);

	return 1;
}

