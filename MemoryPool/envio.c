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


//	tamanio=cant_elementos;
//	memcpy(buffer + desplazamiento, &(tamanio), sizeof(int));
//	desplazamiento+= sizeof(int);

	while(i<cant_elementos)
	{
		tamanio=strlen(array[i])+1;
		memcpy(buffer + desplazamiento, &(tamanio), sizeof(int));
		desplazamiento+= sizeof(int);

		memcpy(buffer + desplazamiento, array[i], tamanio);
		desplazamiento+= tamanio;
		i++;
	}
	return buffer;
}

void* serializar_array_int(int array[], int bytes, int cant_elementos)
{
	void* buffer = malloc(bytes);
	int desplazamiento = 0;
	int tamanio = 0;
	int i = 0;

	while(i<cant_elementos)
	{
		int puerto = array[i];

		memcpy(buffer + desplazamiento, &puerto, sizeof(int));
		desplazamiento+= sizeof(int);
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
	//free(buffer);
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
	//free(buffer);
}

void* serializar_enviar_paquete_describe(int socket_cliente, t_list* metadata)
{
	int cant_elementos = list_size(metadata);

	int i=0;

	send(socket_cliente,&cant_elementos, sizeof(int), MSG_WAITALL);

	while(i<cant_elementos)
	{
		int desplazamiento = 0;
		t_metadata* nodo_metadata = list_get(metadata,i);
		char* nombre_tabla = (nodo_metadata->nombre_tabla);
		char* consistencia = (nodo_metadata->consistencia);

		int tamanio_nombre_tabla = strlen(nombre_tabla) + 1;
		int tamanio_consistencia = strlen(consistencia) + 1;
		int tamanio_total = tamanio_nombre_tabla + tamanio_consistencia+2*sizeof(int);


		void* buffer = malloc(tamanio_nombre_tabla+tamanio_consistencia+2*sizeof(int));

		memcpy(buffer + desplazamiento, &tamanio_nombre_tabla, sizeof(int));
		desplazamiento+= sizeof(int);

		memcpy(buffer + desplazamiento, nombre_tabla, tamanio_nombre_tabla);
		desplazamiento+= tamanio_nombre_tabla;

		memcpy(buffer + desplazamiento, &tamanio_consistencia, sizeof(int));
		desplazamiento+= sizeof(int);

		memcpy(buffer + desplazamiento, consistencia, tamanio_consistencia);
		desplazamiento+= tamanio_consistencia;

		send(socket_cliente,&tamanio_total,sizeof(int),MSG_WAITALL);
		send(socket_cliente,buffer,tamanio_total,MSG_WAITALL);

		i++;
		free(buffer);
	}

}

void* serealizar_seed_completa(int memoria, int puerto, char* IP,int tamanio_total)
{

	int bytes_ip = strlen(IP)+1;
	//3 size of int porque so 2 de memoria y puerto, y otro de tamanio de ip
	void* buffer = malloc(tamanio_total);

	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &bytes_ip, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(buffer + desplazamiento, IP, bytes_ip);
	desplazamiento+= bytes_ip;

	memcpy(buffer + desplazamiento, &memoria, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(buffer + desplazamiento, &puerto, sizeof(int));
	desplazamiento+= sizeof(int);



	return buffer;

}
//---------------------------ENVIOS DE MEMORIAS

void enviar_memorias(int socket_cliente, t_config* config)
{
	char** MEMORY_NUMBERS = config_get_array_value(config, "MEMORY_NUMBERS");
	char** IP_SEEDS = config_get_array_value(config, "IP_SEEDS");
	char** PUERTO_SEEDS = config_get_array_value(config, "PUERTO_SEEDS");

	int cant_elementos = cant_elementos_array(IP_SEEDS);

	int puertos[cant_elementos];
	int memory_number[cant_elementos];

	for (int i=0; i<cant_elementos;i++)
	{
		puertos[i] = atoi(PUERTO_SEEDS[i]);
		memory_number[i] = atoi(MEMORY_NUMBERS[i]);
	}

	send(socket_cliente,&cant_elementos,sizeof(int),MSG_WAITALL);

	for(int j=0;j<cant_elementos;j++)
	{
		int tamanio_total = 3*sizeof(int) + strlen(IP_SEEDS[j])+1;

		void* envio = serealizar_seed_completa(memory_number[j],puertos[j],IP_SEEDS[j],tamanio_total);

		send(socket_cliente,envio,tamanio_total,MSG_WAITALL);
	}
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
