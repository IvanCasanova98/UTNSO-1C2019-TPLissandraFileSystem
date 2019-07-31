#include "recibir.h"

//---------------------------LEVANTAR SERVIDOR

void* servidor(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	int server_fd = iniciar_servidor(parametro->config);

	int cliente_fd = esperar_cliente(parametro->config, server_fd);

	recibir_paquetes(cliente_fd, server_fd, parametro->config, parametro->logger);

    return NULL;
}

//----------------------------RECIBIR PAQUETE

void recibir_paquetes(int cliente_fd, int server_fd, t_config* config, t_log* logger)
{
	int cod_op;
	while(1){

		if (cliente_fd!=0)
		{
			cod_op = recibir_operacion(cliente_fd);
		}
		else
		{
			cliente_fd = esperar_cliente(config, server_fd);
			cod_op = recibir_operacion(cliente_fd);
		}

		switch(cod_op)
		{
		case CREATE:;
			t_paquete_create* paquete_create = deserializar_paquete_create(cliente_fd);

			loggear_paquete_create(paquete_create, logger);

			create(paquete_create, config, logger);

			free(paquete_create->nombre_tabla);
			free(paquete_create->consistencia);
			free(paquete_create);

			break;
		case DROP:;
			int tamanio_buffer;
			char * nombre_tabla = recibir_buffer(&tamanio_buffer, cliente_fd);

			drop(nombre_tabla,config,logger);
			break;
		case DESCRIBE:;

			int tamanio;
			char* buffer = recibir_buffer(&tamanio, cliente_fd);
			if(!strcmp(buffer,"ALL"))
			{
				int size=strlen("ALL")+1;

				t_paquete_describe_lfs* paquete_describe_lfs=malloc(sizeof(uint32_t)+size);
				paquete_describe_lfs->nombre_tabla=malloc(size);
				strcpy(paquete_describe_lfs->nombre_tabla,"ALL");
				paquete_describe_lfs->nombre_tabla_long=size;

				t_list* tabla_particiones = enviar_describe_lissandra(paquete_describe_lfs,config,logger);

				if (tabla_particiones!=NULL){
				serializar_enviar_paquete_describe(cliente_fd, tabla_particiones);
				}else {
					int rta=0;
					send(cliente_fd,&rta,sizeof(int),0);
				}
				free(paquete_describe_lfs->nombre_tabla);
				free(paquete_describe_lfs);
				//BORRAR TABLA DE PARTICIONES

			}
			else
			{
				int size=strlen(buffer)+1;
				t_paquete_describe_lfs* paquete_describe_lfs=malloc(sizeof(t_paquete_describe_lfs)+size);
				paquete_describe_lfs->nombre_tabla=malloc(size);
				strcpy(paquete_describe_lfs->nombre_tabla,buffer);
				paquete_describe_lfs->nombre_tabla_long=size;
				t_list* tabla_particiones = enviar_describe_lissandra(paquete_describe_lfs,config,logger);

				if (tabla_particiones!=NULL){
				serializar_enviar_paquete_describe(cliente_fd, tabla_particiones);
				}else {
					int rta=0;
					send(cliente_fd,&rta,sizeof(int),0);
				}
				//BORRAR TABLA DE PARTICIONES

				free(paquete_describe_lfs->nombre_tabla);
				free(paquete_describe_lfs);
			}

			break;
		case SELECT:;
			t_paquete_select *paquete_select = deserializar_paquete_select(cliente_fd);

			loggear_paquete_select(paquete_select, logger);

			selectf(cliente_fd, paquete_select, config, logger);

			free(paquete_select->nombre_tabla);
			free(paquete_select);

			break;
		case INSERT:;
			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);

			loggear_paquete_insert(paquete_insert, logger);

			insert(paquete_insert, config, logger, 1);

			free(paquete_insert->nombre_tabla);
			free(paquete_insert->value);
			free(paquete_insert);

			break;
		case JOURNAL:
			journal(config,logger);
			//FALTA LOGGEAR EL JOURNAL!!
			//ACUERDENSE DE LOGGEAR LAS QUE NO SE PUDIERON INSERTAR PORQUE NO EXISTE LA TABLA EN EL FS
			break;
		case RUN:
			break;
		case ADD:
			break;
		case HS:
			enviar_memorias(cliente_fd, config);
			cliente_fd=0;
			break;
		case -1:
			cliente_fd=0;
			break;
		default:
			puts("operacion desconocida.");
			break;
		}
	}
}

//----------------------------TIPO DE OPERACION RECIBIDA

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0) return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

//----------------------------DESERIALIZAR PAQUETES



t_paquete_select* deserializar_paquete_select(int socket_cliente)
{
	int desplazamiento = 0;
	size_t tamanioTabla;

	recv(socket_cliente, &tamanioTabla, sizeof(int) ,MSG_WAITALL);

	t_paquete_select *paqueteSelect = malloc(tamanioTabla+sizeof(uint16_t)+sizeof(int));
	paqueteSelect->nombre_tabla = malloc(tamanioTabla);

	void *buffer = malloc(tamanioTabla+sizeof(uint16_t));

	recv(socket_cliente, buffer, tamanioTabla+sizeof(uint16_t) ,MSG_WAITALL);

	memcpy(paqueteSelect->nombre_tabla,buffer + desplazamiento, tamanioTabla);
	desplazamiento+= tamanioTabla;
	memcpy(&(paqueteSelect->valor_key),buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	paqueteSelect->nombre_tabla_long=tamanioTabla;
	free(buffer);
	return paqueteSelect;
}


t_paquete_insert* deserializar_paquete_insert(int socket_cliente)
{
	void *buffer_para_longitudes = malloc(sizeof(uint32_t)*2);

	int desplazamiento = 0;
	size_t tamanioTabla;
	size_t tamanioValue;

	recv(socket_cliente, buffer_para_longitudes, sizeof(uint32_t)*2, MSG_WAITALL);

	memcpy(&tamanioTabla ,buffer_para_longitudes,sizeof(uint32_t));
	memcpy(&tamanioValue ,buffer_para_longitudes+sizeof(uint32_t),sizeof(uint32_t));
	t_paquete_insert *paquete_insert = malloc(tamanioTabla + tamanioValue + sizeof(int)*2 + sizeof(uint16_t)+ sizeof(long long));

	paquete_insert->nombre_tabla = malloc(tamanioTabla);
	paquete_insert->value = malloc(tamanioValue);

	void *buffer = malloc(sizeof(long long)+ sizeof(uint16_t) +tamanioTabla+tamanioValue);

	recv(socket_cliente, buffer, sizeof(long long)+ sizeof(uint16_t) +tamanioTabla+tamanioValue ,MSG_WAITALL);

	memcpy(paquete_insert->nombre_tabla,buffer + desplazamiento,tamanioTabla);
	desplazamiento+= tamanioTabla;
	memcpy(paquete_insert->value,buffer + desplazamiento, tamanioValue);
	desplazamiento+= tamanioValue;
	memcpy(&(paquete_insert->valor_key),buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	memcpy(&(paquete_insert->timestamp),buffer + desplazamiento, sizeof(long long));
	desplazamiento+= sizeof(long long);
	paquete_insert->nombre_tabla_long = tamanioTabla;
	paquete_insert->value_long = tamanioValue;
	free(buffer_para_longitudes);
	free(buffer);
	return paquete_insert;
}

t_paquete_create* deserializar_paquete_create(int socket_cliente)
{
	void *buffer_para_longitudes = malloc(sizeof(uint32_t)*2);

	int desplazamiento = 0;
	size_t tamanio_tabla;
	size_t tamanio_consistencia;

	recv(socket_cliente, buffer_para_longitudes, sizeof(uint32_t)*2 ,MSG_WAITALL);

	memcpy(&tamanio_tabla ,buffer_para_longitudes,sizeof(uint32_t));
	memcpy(&tamanio_consistencia ,buffer_para_longitudes+sizeof(uint32_t),sizeof(uint32_t));
	t_paquete_create* paquete_create = malloc(tamanio_tabla + tamanio_consistencia + sizeof(uint32_t)*2 + sizeof(int)*2);

	paquete_create->nombre_tabla = malloc(tamanio_tabla);
	paquete_create->consistencia = malloc(tamanio_consistencia);

	void *buffer = malloc(tamanio_tabla + tamanio_consistencia + sizeof(int)*2);

	recv(socket_cliente, buffer, sizeof(int)*2 +tamanio_tabla+tamanio_consistencia ,MSG_WAITALL);

	memcpy(paquete_create->nombre_tabla,buffer + desplazamiento, tamanio_tabla);
	desplazamiento+= tamanio_tabla;

	memcpy(paquete_create->consistencia,buffer + desplazamiento, tamanio_consistencia);
	desplazamiento+= tamanio_consistencia;

	memcpy(&(paquete_create->particiones),buffer + desplazamiento, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(&(paquete_create->tiempo_compactacion),buffer + desplazamiento, sizeof(int));
	desplazamiento+= sizeof(int);

	paquete_create->nombre_tabla_long = tamanio_tabla;
	paquete_create->consistencia_long = tamanio_consistencia;

	free(buffer_para_longitudes);
	free(buffer);
	return paquete_create;
}

t_list* deserializar_respuesta_describe(int conexion){
	int cantidadDeTablas;
	recv(conexion, &cantidadDeTablas, sizeof(int) ,MSG_WAITALL);
	int i=0;
	t_list* listaDescribe= list_create();
	while(i<cantidadDeTablas)
	{


		int longNombreTabla;
		int longConsistencia;

		int desplazamiento=0;
		recv(conexion, &longNombreTabla, sizeof(int) ,MSG_WAITALL);
		recv(conexion, &longConsistencia, sizeof(int) ,MSG_WAITALL);

		void* buffer=malloc(sizeof(int)*2+longNombreTabla+longConsistencia);
		int tiempoCompac;

		recv(conexion, buffer,sizeof(int)*2+longNombreTabla+longConsistencia ,MSG_WAITALL);

		t_metadata* metadata= malloc(sizeof(longConsistencia)+sizeof(int)*2);
		metadata->nombre_tabla=malloc(longNombreTabla);
		metadata->consistencia=malloc(longConsistencia);

		memcpy(metadata->nombre_tabla, buffer + desplazamiento, longNombreTabla);
		desplazamiento+= longNombreTabla;
		memcpy(metadata->consistencia,buffer + desplazamiento, longConsistencia);
		desplazamiento+= longConsistencia;
		memcpy(&(metadata->particiones),buffer + desplazamiento, sizeof(int));
		desplazamiento+= sizeof(int);
		memcpy(&tiempoCompac,buffer + desplazamiento, sizeof(int));
		desplazamiento+= sizeof(int);

		list_add(listaDescribe,metadata);

		i++;
		free(buffer);
	}

	return listaDescribe;
}

t_pagina* deserializar_pagina (int socket_cliente)
{
	/*BUFFER:
	 * BIT DE ERROR
	 * TAMAÑO VALUE
	 * VALUE
	 * TIMESTAMP
	 */
	int desplazamiento = 0;
	size_t tamanio_paquete;
	uint16_t bit_error;
	size_t tamanio_value;
	long long timestamp;

	t_pagina* pagina;

//	puts("EN DESERIALIZAR");


//	puts("RECV 1");


//	puts("RECV 2");
	recv(socket_cliente, &bit_error, sizeof(uint16_t) ,MSG_WAITALL);


	if(bit_error == 0)
	{

		recv(socket_cliente, &tamanio_paquete, sizeof(size_t) ,MSG_WAITALL);
		void *buffer = malloc(tamanio_paquete);
//		puts("BIT DE ERROR 0");

		recv(socket_cliente, buffer, tamanio_paquete ,MSG_WAITALL);

		memcpy(&(tamanio_value),buffer + desplazamiento, sizeof(size_t));
		desplazamiento+= sizeof(size_t);

		char* value = malloc(tamanio_value);
		memcpy(value,buffer + desplazamiento, tamanio_value);
		desplazamiento+= tamanio_value;

		memcpy(&timestamp,buffer + desplazamiento, sizeof(long long));
		desplazamiento+= sizeof(long long);

		//pagina= malloc(sizeof(uint16_t)+tamanio_value+sizeof(long long));
		pagina= malloc(sizeof(t_pagina));


		pagina->value = malloc(tamanio_value);


		strcpy(pagina->value,value);
		pagina->timestamp = timestamp;
		free(value);
		free(buffer);


	}
	else
	{
//		puts("BIT DE ERROR 1");
		char* value = string_new();
		pagina= malloc(sizeof(uint16_t)+strlen("Pagina no encontrada")+1+sizeof(long long));
		pagina->value=malloc(strlen("Pagina no encontrada")+1);
		strcpy(pagina->value,"Pagina no encontrada");
		pagina->timestamp = 0;
	}



	return pagina;
}

//------------------RECIBIR MENSAJES------------------
void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), 0);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, 0);

	return buffer;
	free(buffer);
}
void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);

	t_log* logger = iniciar_logger();
	log_info(logger, "Value recibido: %s\n", buffer);
    log_destroy(logger);

	free(buffer);
}


void* recibir_mensaje_para_kernel(int socket_cliente){
	int size;
	void* buffer = recibir_buffer(&size, socket_cliente);

	t_log* logger = iniciar_logger();
	log_info(logger, "\nValue recibido del filesystem: %s", buffer);
	log_destroy(logger);

	return buffer;

}


