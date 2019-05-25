#include "envio.h"

//----------------------------SERIALIZAR MENSAJE

void* serializar_mensaje(char* value, int bytes)
{
	void* buffer = malloc(bytes);
	int desplazamiento = 0;

	int tamanio=strlen(value)+1;
	memcpy(buffer + desplazamiento, &(tamanio), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(buffer + desplazamiento, value, tamanio);

	return buffer;
}

void* serializar_array(char** array, int bytes, int cant_elementos)
{
	void* buffer = malloc(bytes);
	int desplazamiento = 0;
	int tamanio = 0;
	int i = 0;


	tamanio=cant_elementos;
	memcpy(buffer + desplazamiento, &(tamanio), sizeof(int));
	desplazamiento+= sizeof(int);

	while(i<cant_elementos)
	{
		tamanio=strlen(array[i]) + 5;
		memcpy(buffer + desplazamiento, &(tamanio), sizeof(int));
		desplazamiento+= sizeof(int);

		memcpy(buffer + desplazamiento, array[i], tamanio);
		desplazamiento+= tamanio;
		printf("\nIP N° %d: %s", i,array[i]);
		i++;
	}

	return buffer;
	free(buffer);
}

//----------------------------SERIALIZAR PAQUETES

void* serializar_paquete_select(t_paquete_select* paquete)
{
	void* buffer = malloc(sizeof(uint32_t) + sizeof(uint16_t)  + paquete->nombre_tabla_long);
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

	return buffer;
	free(buffer);
}

//---------------------------ENVIOS DE MEMORIAS

void enviar_memorias(int socket_cliente, t_config* config)
{
	char** IP_SEEDS = config_get_array_value(config, "IP_SEEDS");
	char** PUERTO_SEEDS = config_get_array_value(config, "PUERTO_SEEDS");

	int cant_elementos = cant_elementos_array(IP_SEEDS);

	int bytes_IP = tamanio_array(IP_SEEDS);
	int bytes_PUERTO = tamanio_array(PUERTO_SEEDS);

	void* enviar_IP_SEEDS = serializar_array(IP_SEEDS, bytes_IP, cant_elementos);
	void* enviar_PUERTO_SEEDS = serializar_array(PUERTO_SEEDS, bytes_PUERTO, cant_elementos);

	send(socket_cliente,enviar_IP_SEEDS,bytes_IP,0);
	send(socket_cliente,enviar_PUERTO_SEEDS,bytes_PUERTO,0);
}

//----------------------------ENVIAR PAQUETES

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente, t_log* logger)
{
	int bytes = 8 + paquete->nombre_tabla_long;
	void* a_enviar = serializar_paquete_select(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE SELECT.");
	else
	{
			log_info(logger, "PAQUETE SELECT ENVIADO");
	}
	free(a_enviar);
}

void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente, t_log* logger)
{
	int bytes = sizeof(int)*4 +paquete->nombre_tabla_long + paquete->value_long;
	void* a_enviar = serializar_paquete_insert(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE INSERT.");
	else {
			log_info(logger, "PAQUETE INSERT ENVIADO");
	}
	free(a_enviar);
}

//---------------------------ENVIOS DE SERVIDOR A LISSANDRA

void enviar_select_lissandra(t_paquete_select* paquete, t_config* config, t_log* logger)
{
	int conexion = iniciar_conexion(config);
	int cod = 3;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_select(paquete, conexion, logger);}
	free(paquete);

	terminar_conexion(conexion);
}

void enviar_insert_lissandra(t_paquete_insert* paquete, t_config* config, t_log* logger)
{
	int conexion = iniciar_conexion(config);
	int cod = 4;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_insert(paquete, conexion, logger);}
	free(paquete);

	terminar_conexion(conexion);
}
