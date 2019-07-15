#include "Lissandra.h"


//----------------------------RECIBIR PAQUETE
void* recibir_paquetes(void *arg)
{
	int cliente_fd = *((int *) arg);
	free(arg);

	//mutex
	int cod_op = recibir_operacion(cliente_fd);

		if (cliente_fd==0){
			printf("Error al conectarse para intentar recibir paquete\ņ");
			pthread_exit(NULL);
		}

		switch(cod_op){

		case CREATE:;
			t_paquete_create_de_mp* paquete_create_de_mp = deserializar_paquete_create_de_mp(cliente_fd);
			t_paquete_create* paquete_create_fs;
			paquete_create_fs = adaptadorDePaquete(paquete_create_de_mp);
			if(paquete_create_fs==NULL) break;
			APIcreate(paquete_create_fs);

			loggear_request_create(paquete_create_fs);
			break;
		case DROP:;
			t_paquete_drop* paquete_drop = deserializar_paquete_drop(cliente_fd);
			if(paquete_drop==NULL) break;
			loggear_request_drop(paquete_drop);
			APIdrop(paquete_drop);
			break;

		case DESCRIBE:;
			log_info(logger, "Se recibio paquete tipo: DESCRIBE");
			break;

		case SELECT:;

			t_paquete_select *paquete_select=deserializar_paquete_select(cliente_fd);
			if(paquete_select==NULL) break;
			loggear_request_select(paquete_select);
			APIselect(paquete_select);

			break;
		case INSERT:;

			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);
			if(paquete_insert==NULL) break;
			APIinsert(paquete_insert);

			break;
		case JOURNAL:;

			//AGREGE ACA PARA SALIR. !!
			log_info(logger, "FIN CONEXION.\n");
			cliente_fd=0;
			log_info(logger, "Se recibio paquete tipo: JOURNAL");
			pthread_exit(NULL);

			break;
		case RUN:;
			log_info(logger, "Se recibio paquete tipo: RUN");
			break;
		case ADD:;
			log_info(logger, "Se recibio paquete tipo: ADD");
			break;
		case -1:
			log_info(logger, "FIN CONEXION.\n");
			cliente_fd=0;
			pthread_exit(NULL);
			break;
		default:
			log_warning(logger, "Operacion desconocida.");
			break;
		}
		//salgo mutex
		pthread_exit(NULL);


}

//----------------------------TIPO DE OPERACION RECIBIDA
int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0) return cod_op;
	else
	{
		close(socket_cliente);
		puts(" *** No se recibio correctamente el COD OP ***");
		return -1;
	}
}


//----------------------------DESERIALIZAR PAQUETES
t_paquete_select* deserializar_paquete_select(int socket_cliente){


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

t_paquete_create_de_mp* deserializar_paquete_create_de_mp(int socket_cliente)
{
	void *buffer_para_longitudes = malloc(sizeof(uint32_t)*2);

	int desplazamiento = 0;
	size_t tamanio_tabla;
	size_t tamanio_consistencia;

	recv(socket_cliente, buffer_para_longitudes, sizeof(uint32_t)*2 ,MSG_WAITALL);

	memcpy(&tamanio_tabla ,buffer_para_longitudes,sizeof(uint32_t));
	memcpy(&tamanio_consistencia ,buffer_para_longitudes+sizeof(uint32_t),sizeof(uint32_t));
	t_paquete_create_de_mp* paquete_create = malloc(tamanio_tabla + tamanio_consistencia + sizeof(uint32_t)*2 + sizeof(uint16_t)*2);

	paquete_create->nombre_tabla = malloc(tamanio_tabla);
	paquete_create->consistencia = malloc(tamanio_consistencia);

	void *buffer = malloc(tamanio_tabla + tamanio_consistencia + sizeof(uint16_t)*2);

	recv(socket_cliente, buffer, sizeof(uint16_t)*2 +tamanio_tabla+tamanio_consistencia ,MSG_WAITALL);

	memcpy(paquete_create->nombre_tabla,buffer + desplazamiento, tamanio_tabla);
	desplazamiento+= tamanio_tabla;

	memcpy(paquete_create->consistencia,buffer + desplazamiento, tamanio_consistencia);
	desplazamiento+= tamanio_consistencia;

	memcpy(&(paquete_create->particiones),buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);

	memcpy(&(paquete_create->tiempo_compactacion),buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);

	paquete_create->nombre_tabla_long = tamanio_tabla;
	paquete_create->consistencia_long = tamanio_consistencia;

	free(buffer_para_longitudes);
	free(buffer);
	return paquete_create;
}

//FALTA PROBARLA ESTA
//*******************************************************************
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

t_paquete_create* adaptadorDePaquete(t_paquete_create_de_mp* paquete_create_mp){
	t_paquete_create* paquete_adaptado = crear_paquete_create(
			paquete_create_mp->nombre_tabla,
			pasarAConsistenciaINT(paquete_create_mp->consistencia),
			paquete_create_mp->particiones,
			paquete_create_mp->tiempo_compactacion);

//	memcpy(paquete_adaptado->nombre_tabla,paquete_create_mp->nombre_tabla,sizeof(paquete_create_mp->nombre_tabla));
//	paquete_adaptado->metadata.particiones = paquete_create_mp->particiones;
//	paquete_adaptado->metadata.tiempo_de_compactacion = paquete_create_mp->tiempo_compactacion;
//	paquete_adaptado->metadata.consistencia = pasarAConsistenciaINT(paquete_create_mp->consistencia);

	return paquete_adaptado;
}

