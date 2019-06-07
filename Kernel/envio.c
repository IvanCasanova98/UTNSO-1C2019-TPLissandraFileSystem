#include "envio.h"

//----------------------------ARCHIVOS LOGGER Y CONFIG

t_log* iniciar_logger() {
	return log_create("Kernel.log", "Kernel", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	return config_create("Kernel.config");
}

//----------------------------SERIALIZAR PAQUETES

void* serializar_paquete_select(t_paquete_select* paquete)
{

	void* buffer = malloc(sizeof(uint32_t) + sizeof(uint16_t)  + paquete->nombre_tabla_long);
	//REVISAR MALLOC

	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	memcpy(buffer + desplazamiento, &paquete->valor_key, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	return buffer;
	free(buffer);
}

void* serializar_paquete_insert(t_paquete_insert* paquete)
{
	void * buffer = malloc(sizeof(uint16_t)+sizeof(uint32_t)*2 + sizeof(long long) +paquete->nombre_tabla_long +paquete->value_long);
	//revisar malloc

	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, &paquete->value_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	memcpy(buffer + desplazamiento, paquete->value, paquete->value_long);
	desplazamiento+= paquete->value_long;

	memcpy(buffer + desplazamiento, &(paquete->valor_key), sizeof(uint16_t));
	desplazamiento+=  sizeof(uint16_t);

	memcpy(buffer + desplazamiento, &(paquete->timestamp), sizeof(long long));
	desplazamiento+=  sizeof(long long);

	return buffer;
	free(buffer);

}

void* serializar_paquete_create(t_paquete_create* paquete)
{
	void* buffer = malloc(paquete->nombre_tabla_long + paquete->consistencia_long + sizeof(uint32_t)*2 + sizeof(uint16_t)*2); //Mandar por parametro
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(buffer + desplazamiento, &paquete->consistencia_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	memcpy(buffer + desplazamiento, paquete->consistencia, paquete->consistencia_long);
	desplazamiento+= paquete->consistencia_long;

	memcpy(buffer + desplazamiento, &paquete->particiones, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);

	memcpy(buffer + desplazamiento, &paquete->tiempo_compactacion, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);

	return buffer;
	free(buffer);
}

//----------------------------ENVIAR PAQUETES

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente) //DEBE LLEGAR LOGGER
{
	int bytes = sizeof(int) + sizeof(uint16_t) + paquete->nombre_tabla_long;
	void* a_enviar = serializar_paquete_select(paquete);
	//printf("%s %d",paquete->nombre_tabla,paquete->valor_key);
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
	int bytes = sizeof(long long) + sizeof(int)*2 +paquete->nombre_tabla_long + paquete->value_long +sizeof(uint16_t);
	void* a_enviar = serializar_paquete_insert(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE INSERT.");
	else {
			t_log* logger = iniciar_logger();
			log_info(logger, "PAQUETE INSERT ENVIADO");
		    log_destroy(logger);
	}

	free(a_enviar);
}


void enviar_paquete_create(t_paquete_create* paquete, int socket_cliente)
{
	int bytes = paquete->nombre_tabla_long + paquete->consistencia_long + sizeof(uint32_t)*2 + sizeof(uint16_t)*2;
	void* a_enviar = serializar_paquete_create(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE CREATE.");
	else {
			t_log* logger = iniciar_logger();
			log_info(logger, "PAQUETE CREATE ENVIADO");
		    log_destroy(logger);
	}

	free(a_enviar);
}

