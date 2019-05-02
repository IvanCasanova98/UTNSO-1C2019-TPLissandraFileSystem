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
	//PARA MI MAGIC ES EL BUFFER !
	size_t longitudNombreTabla = strlen(paquete->nombre_tabla);
	void * buffer = malloc(11);
	int desplazamiento = 0;
	memcpy(buffer + desplazamiento, &(paquete->nombre_tabla), 7);
	desplazamiento+= 7;
	memcpy(buffer + desplazamiento, &(paquete->valor_key), 4);
	desplazamiento+= 4;
	return buffer; //es void y se esta tornando mmm
	free(buffer);
}

//void* serializar_paquete_insert(t_paquete_insert* paquete, int bytes)
//{
//	void * magic = malloc(bytes);
//	int desplazamiento = 0;
//
//	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
//	desplazamiento+= sizeof(int);
//	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
//	desplazamiento+= sizeof(int);
//	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
//	desplazamiento+= paquete->buffer->size;
//
//	return magic;
//}

//----------------------------ENVIAR PAQUETES

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente)
{
	printf("ola");
	size_t longitudNombreTabla = strlen(paquete->nombre_tabla);
	printf("ola");
	int bytes = 11;

	printf("ola");
	void* a_enviar = serializar_paquete_select(paquete);

	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE SELECT.");
	else {
			t_log* logger = iniciar_logger();
			log_info(logger, "PAQUETE SELECT ENVIADO");
		    log_destroy(logger);

	}

	free(a_enviar);

//    eliminar_paquete_select(paquete);
}

//void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente)
//{
//	int bytes = paquete->buffer->size + 2*sizeof(int);
//	void* a_enviar = serializar_paquete_insert(paquete, bytes);
//
//	send(socket_cliente, a_enviar, bytes, 0);
//
//	t_log* logger = iniciar_logger();
//	log_info(logger, "PAQUETE INSERT ENVIADO");
//    log_destroy(logger);
//
//	free(a_enviar);
//
//    eliminar_paquete_insert(paquete);
//}

