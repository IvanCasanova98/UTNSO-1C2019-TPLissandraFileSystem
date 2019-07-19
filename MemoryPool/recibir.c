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

			break;
		case DROP:
			break;
		case DESCRIBE:;
			int tamanio;
			char* buffer = recibir_buffer(&tamanio, cliente_fd);
			if(!strcmp(buffer,"ALL"))
			{
				t_list* tabla_particiones = get_tabla_particiones();
				serializar_enviar_paquete_describe(cliente_fd, tabla_particiones);
			}
			else
			{
				t_list* nodo_metadata = get_nodo_metadata(buffer);
				serializar_enviar_paquete_describe(cliente_fd, nodo_metadata);
			}

			break;
		case SELECT:;
			t_paquete_select *paquete_select = deserializar_paquete_select(cliente_fd);

			loggear_paquete_select(paquete_select, logger);

			selectf(cliente_fd, paquete_select, config, logger);
			break;
		case INSERT:;
			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);

			loggear_paquete_insert(paquete_insert, logger);

			insert(paquete_insert, config, logger);
			break;
		case JOURNAL:
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

t_paquete_drop* deserializar_paquete_drop(int socket_cliente){
	int desplazamiento = 0;
	size_t tamanioNombreTabla;

	recv(socket_cliente, &tamanioNombreTabla, sizeof(int) ,MSG_WAITALL);

	t_paquete_drop* paqueteDrop = malloc(tamanioNombreTabla+sizeof(uint32_t));
	paqueteDrop->nombre_tabla = malloc(tamanioNombreTabla);

	void *buffer = malloc(tamanioNombreTabla);

	recv(socket_cliente, buffer, tamanioNombreTabla ,MSG_WAITALL);

	memcpy(paqueteDrop->nombre_tabla,buffer + desplazamiento, tamanioNombreTabla);
	desplazamiento+= tamanioNombreTabla;

	free(buffer);
	return paqueteDrop;

}

t_paquete_select* deserializar_paquete_select(int socket_cliente)
{
	int desplazamiento = 0;
	size_t tamanioTabla;

	recv(socket_cliente, &tamanioTabla, sizeof(int) ,MSG_WAITALL);

	t_paquete_select *paqueteSelect= malloc(tamanioTabla+sizeof(uint16_t)+sizeof(int));
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

t_dictionary* deserializar_respuesta_describe(int conexion){
	t_dictionary* diccionarioDescribe=dictionary_create();

	int cantidadDeTablas;
	recv(conexion, &cantidadDeTablas, sizeof(int) ,MSG_WAITALL);
	int i=0;

	while(i<cantidadDeTablas)
	{

		int longNombreTabla;
		int longConsistencia;

		int desplazamiento=0;
		recv(conexion, &longNombreTabla, sizeof(int) ,MSG_WAITALL);
		recv(conexion, &longConsistencia, sizeof(int) ,MSG_WAITALL);

		void* buffer=malloc(sizeof(int)*2+longNombreTabla+longConsistencia);


		recv(conexion, buffer,sizeof(int)*2+longNombreTabla+longConsistencia ,MSG_WAITALL);


		char* nombreTabla=malloc(longNombreTabla);
		t_metadataDescribe* metadata= malloc(sizeof(longConsistencia)+sizeof(int)*2); //CAMBIE ESTE MALLOC
		metadata->consistencia=malloc(longConsistencia);

		memcpy(nombreTabla, buffer + desplazamiento, longNombreTabla);
		desplazamiento+= longNombreTabla;
		memcpy(metadata->consistencia,buffer + desplazamiento, longConsistencia);
		desplazamiento+= longConsistencia;
		memcpy(&(metadata->particiones),buffer + desplazamiento, sizeof(int));
		desplazamiento+= sizeof(int);
		memcpy(&(metadata->tiempo_de_compactacion),buffer + desplazamiento, sizeof(int));
		desplazamiento+= sizeof(int);

		dictionary_put(diccionarioDescribe,nombreTabla,metadata);

		i++;
	}

return diccionarioDescribe;
}

respuestaSELECT_FS *deserializar_respuesta_select(int socket_cliente){

	int desplazamiento = 0;
	uint16_t rta;

	recv(socket_cliente, &rta, sizeof(uint16_t) ,MSG_WAITALL);
	desplazamiento+= sizeof(uint16_t);

	if(rta != 30 ) { //HUBO FALLO.

		respuestaSELECT_FS* respuestaSELECT = malloc(sizeof(uint32_t) + sizeof(uint16_t));
		respuestaSELECT->rta = rta;

		respuestaSELECT->keyHallada = malloc(1);
		respuestaSELECT->keyHallada = "";
		respuestaSELECT->tamanio_key=1;

		return respuestaSELECT;
	}
	else if(rta == 30) //EXITO
	{
		size_t tamanioKey;
		recv(socket_cliente, &tamanioKey, sizeof(uint32_t) ,MSG_WAITALL);

		respuestaSELECT_FS* respuestaSELECT= malloc(tamanioKey+sizeof(uint16_t)+sizeof(uint32_t));

		void* buffer = malloc(tamanioKey);
		respuestaSELECT->keyHallada= malloc(tamanioKey);

		recv(socket_cliente, buffer, tamanioKey ,MSG_WAITALL);


		memcpy(respuestaSELECT->keyHallada,buffer, tamanioKey);
		respuestaSELECT->rta = rta;
		respuestaSELECT->tamanio_key= tamanioKey;

		free(buffer);
		return respuestaSELECT;
	} else{
		respuestaSELECT_FS* respuestaSELECT = malloc(sizeof(uint32_t) + sizeof(uint16_t));
				respuestaSELECT->rta = 34;

				respuestaSELECT->keyHallada = malloc(1);
				respuestaSELECT->keyHallada = "";
				respuestaSELECT->tamanio_key=1;
		return respuestaSELECT;
	}

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
