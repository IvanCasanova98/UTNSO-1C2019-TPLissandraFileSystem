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

	void* buffer = malloc(sizeof(uint32_t) + sizeof(int)  + paquete->nombre_tabla_long);
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	memcpy(buffer + desplazamiento, &paquete->valor_key, 4);
	desplazamiento+= 4;
	return buffer;
	free(buffer);
}

void* serializar_paquete_insert(t_paquete_insert* paquete)
{
	void * buffer = malloc(sizeof(uint32_t)*2 + sizeof(int)*2 +paquete->nombre_tabla_long +paquete->value_long);
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, &paquete->value_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	memcpy(buffer + desplazamiento, paquete->value, paquete->value_long);
	desplazamiento+= paquete->value_long;

	memcpy(buffer + desplazamiento, &(paquete->valor_key), 4);
	desplazamiento+=  sizeof(int);

	memcpy(buffer + desplazamiento, &(paquete->timestamp), 4);
	desplazamiento+=  sizeof(int);

	return buffer; //es void y se esta tornando mmm
	free(buffer);

}

//----------------------------ENVIAR PAQUETES

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente)
{
	int bytes = 8 + paquete->nombre_tabla_long;
	void* a_enviar = serializar_paquete_select(paquete);
	printf("%s %d",paquete->nombre_tabla,paquete->valor_key);
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
	int bytes = sizeof(int)*4 +paquete->nombre_tabla_long + paquete->value_long;
	void* a_enviar = serializar_paquete_insert(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE INSERT.");
	else {
			t_log* logger = iniciar_logger();
			log_info(logger, "PAQUETE INSERT ENVIADO");
		    log_destroy(logger);
	}

	free(a_enviar);
}


