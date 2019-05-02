/*
 * envio.c
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */
#include "envio.h"

t_log* iniciar_logger() {
	return log_create("Kernel.log", "Kernel", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	return config_create("Kernel.config");
}

//----------------------------SERIALIZAR PAQUETES

void* serializar_paquete_select(t_paquete_select* paquete)
{
	void * buffer = malloc(11);
	int desplazamiento = 0;
	memcpy(buffer + desplazamiento, &(paquete->nombre_tabla), 7);
	desplazamiento+= 7;
	memcpy(buffer + desplazamiento, &(paquete->valor_key), 4);
	desplazamiento+= 4;
	return buffer; //es void y se esta tornando mmm
	free(buffer);
}

void* serializar_paquete_insert(t_paquete_insert* paquete)
{
	void * buffer = malloc(35);
	int desplazamiento = 0;
	memcpy(buffer + desplazamiento, &(paquete->nombre_tabla), 7);
	desplazamiento+= 7;
	memcpy(buffer + desplazamiento, &(paquete->valor_key), 4);
	desplazamiento+= 4;
	memcpy(buffer + desplazamiento, &(paquete->value), 20);
	desplazamiento+= 20;
	memcpy(buffer + desplazamiento, &(paquete->timestamp), 4);
	desplazamiento+= 4;
	return buffer; //es void y se esta tornando mmm
	free(buffer);

}

//----------------------------ENVIAR PAQUETES

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente)
{
	int bytes = 11;
	void* a_enviar = serializar_paquete_select(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE SELECT.");
	else {
			t_log* logger = iniciar_logger();
			log_info(logger, "PAQUETE SELECT ENVIADO");
		    log_destroy(logger);

	}

	free(a_enviar);
}

void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente)
{
	int bytes = 35;
	void* a_enviar = serializar_paquete_insert(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE INSERT.");
	else {
			t_log* logger = iniciar_logger();
			log_info(logger, "PAQUETE INSERT ENVIADO");
		    log_destroy(logger);
	}

	free(a_enviar);
}
