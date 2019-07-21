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

void* serializar_paquete_drop(t_paquete_drop* paquete){

	void* buffer = malloc(paquete->nombre_tabla_long + sizeof(uint32_t));
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;


	return buffer;

}

void* serializar_paquete_describe(t_paquete_describe_lfs* paquete){

	void* buffer = malloc(paquete->nombre_tabla_long + sizeof(uint32_t));
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	return buffer;
}

void* serializar_paquete_insert(t_paquete_insert* paquete)
{
	void * buffer = malloc(sizeof(uint16_t)+sizeof(uint32_t)*2 + sizeof(long long) +paquete->nombre_tabla_long +paquete->value_long);
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
	//free(buffer);
}

void* serializar_paquete_create(t_paquete_create* paquete)
{
	void* buffer = malloc(paquete->nombre_tabla_long + paquete->consistencia_long + sizeof(uint32_t)*2 + sizeof(int)*2); //Mandar por parametro
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &paquete->nombre_tabla_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, &paquete->consistencia_long, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	memcpy(buffer + desplazamiento, paquete->nombre_tabla, paquete->nombre_tabla_long);
	desplazamiento+= paquete->nombre_tabla_long;

	memcpy(buffer + desplazamiento, paquete->consistencia, paquete->consistencia_long);
	desplazamiento+= paquete->consistencia_long;

	memcpy(buffer + desplazamiento, &paquete->particiones, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(buffer + desplazamiento, &paquete->tiempo_compactacion, sizeof(int));
	desplazamiento+= sizeof(int);

	return buffer;
	free(buffer);
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

	free(a_enviar);
}

void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente, t_log* logger)
{
	int bytes = sizeof(uint32_t)*2+sizeof(uint16_t) +paquete->nombre_tabla_long + paquete->value_long + sizeof(long long);
	void* a_enviar = serializar_paquete_insert(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE INSERT.");

	free(a_enviar);
}

void enviar_paquete_create(t_paquete_create* paquete, int socket_cliente,t_log* logger)
{
	int bytes = paquete->nombre_tabla_long + paquete->consistencia_long + sizeof(uint32_t)*2 + sizeof(int)*2;
	void* a_enviar = serializar_paquete_create(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE CREATE.");
	free(a_enviar);
}

void enviar_respuesta_create(int conexion, int booleano, char* nombre_tabla)
{
	char* string = string_new();
	if(booleano)
	{
		strcpy(string,"Ya existe la tabla: ");
	}
	else
	{
		strcpy(string,"Tabla creada: ");
	}
	char* value = strcat(string, nombre_tabla);
	int tamanio_value = strlen(value) + 1;

//	printf("VALUE A ENVIAR CREATE: %s", value);

	send(conexion, &tamanio_value, sizeof(int), 0);
	send(conexion, value, tamanio_value, 0);
}

void enviar_paquete_drop(t_paquete_drop* paquete, int socket_cliente,t_log* logger){
	int bytes = paquete->nombre_tabla_long + sizeof(uint32_t);
	void* a_enviar = serializar_paquete_drop(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE DROP.");

	free(a_enviar);
}

void enviar_paquete_describe(t_paquete_describe_lfs* paquete,int socket_cliente, t_log* logger){

	int bytes = paquete->nombre_tabla_long + sizeof(uint32_t);
	void* a_enviar = serializar_paquete_describe(paquete);
	if ( send(socket_cliente, a_enviar, bytes, 0) <= 0) puts("Error en envio de PAQUETE DESCRIBE.");

	free(a_enviar);
}

//---------------------------ENVIOS DE SERVIDOR A LISSANDRA
t_list* enviar_describe_lissandra(t_paquete_describe_lfs* paquete,t_config* config,t_log* logger)
{
	int conexion = iniciar_conexion(config);
	int cod = 2;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_describe(paquete, conexion, logger);}
	free(paquete);

	//------RESPUESTA DE LISSANDRA:

	uint16_t rta;
	recv(conexion, &rta, sizeof(uint16_t) ,0);
	if(rta==20 || rta==25){
		protocolo_respuesta(rta,logger);
		t_list* lista=deserializar_respuesta_describe(conexion);
		loggearListaMetadatas(lista);
		terminar_conexion(conexion);
		return lista;
	}else{
	//	protocolo_respuesta(rta,logger);
		terminar_conexion(conexion);
		return NULL;
	}
}


void enviar_select_lissandra(int conexion, t_paquete_select* paquete, t_log* logger)
{
	int cod = 3;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_select(paquete, conexion, logger);}
	free(paquete);
}

void enviar_insert_lissandra(t_paquete_insert* paquete, t_config* config, t_log* logger)
{

	int conexion;
	conexion = iniciar_conexion(config);

	int cod = 4;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_insert(paquete, conexion, logger);}
	free(paquete);

	//------RESPUESTA DE LISSANDRA:

	//------RESPUESTA DE LISSANDRA:
//	void* respuesta = recibir_mensaje_para_kernel(conexion);

//	uint16_t rta;
//	recv(conexion, &rta, sizeof(uint16_t) ,MSG_WAITALL);
	//protocolo_respuesta(rta,logger);

	terminar_conexion(conexion);

}

void enviar_create_lissandra(t_paquete_create* paquete,t_config* config,t_log* logger)
{
	int conexion = iniciar_conexion(config);
	int cod = 0;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
			puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_create(paquete, conexion, logger);}
	free(paquete);

	//------RESPUESTA DE LISSANDRA:
//	uint16_t rta;
//	recv(conexion, &rta,sizeof(uint16_t) ,0);

//	protocolo_respuesta(rta,logger);

	terminar_conexion(conexion);
}

void enviar_drop_lissandra(t_paquete_create* paquete,t_config* config,t_log* logger)
{
	int conexion = iniciar_conexion(config);
	int cod = 1;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
			puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_drop(paquete, conexion, logger);}
	free(paquete);

	//------RESPUESTA DE LISSANDRA:

	uint16_t rta;
	recv(conexion, &rta, sizeof(uint16_t) ,0);
	//protocolo_respuesta(rta,logger);

	terminar_conexion(conexion);

}

int recibir_max_value_del_fs(t_config* config,t_log* logger)
{
	int conexion = iniciar_conexion(config);
	int cod = 8;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
			puts("Error en envio de CODIGO DE OPERACION.");

	//------RESPUESTA DE LISSANDRA:
	int max_value_lfs;
	recv(conexion, &max_value_lfs, sizeof(int) ,0);
	terminar_conexion(conexion);
	return max_value_lfs;

}

//----------------------------------------------------------LOGGEAR METADATAS DE LFS
void loggearListaMetadatas(t_list * metadatas){
	list_iterate(metadatas,loggearMetadataTablas);
}

void loggearMetadataTablas(void* elemento){

	t_log* logger= iniciar_logger();
	t_metadata* metadataDeTablaPedida= (t_metadata*)elemento;
	log_info(logger,"%s %s %d",metadataDeTablaPedida->nombre_tabla,metadataDeTablaPedida->consistencia,metadataDeTablaPedida->particiones);
	log_destroy(logger);

}

