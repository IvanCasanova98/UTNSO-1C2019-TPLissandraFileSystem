#include "Lissandra.h"


//----------------------------RECIBIR PAQUETE
void* recibir_paquetes(void *arg)
{
	int cliente_fd = *((int *) arg);
	//free(arg);
	uint16_t rta;

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


			if(paquete_create_fs!=NULL)
			{
//				rta = 93;

//				if (send(cliente_fd, &rta, sizeof(uint16_t), MSG_DONTWAIT) <= 0)
//						puts("Error en envio de CODIGO DE RESPUESTA.");
				break;
				APIcreateRESPUESTA(paquete_create_fs,cliente_fd);
			//void* buffer = malloc(sizeof(uint16_t));
			//memcpy(buffer, &rta, sizeof(uint16_t));
//			if (send(cliente_fd, &rta,sizeof(uint16_t), 0) <= 0)
//				puts("Error en envio de CODIGO DE RESPUESTA.");
			//loggear_request_create_mp(paquete_create_de_mp);

			}
			break;

		case DROP:;
			t_paquete_drop* paquete_drop = deserializar_paquete_drop(cliente_fd);
			if(paquete_drop!=NULL)
			{
//				rta = 13;
//				if (send(cliente_fd, &rta, sizeof(uint16_t), 0) <= 0)
//					puts("Error en envio de CODIGO DE RESPUESTA.");
//				break;
			APIdropRESPUESTA(paquete_drop,cliente_fd);
			loggear_request_drop(paquete_drop);
			}

//			if (send(cliente_fd, &rta, sizeof(uint16_t), 0) <= 0)
//				puts("Error en envio de CODIGO DE RESPUESTA.");
			break;

		case DESCRIBE:;
			t_paquete_describe* paquete_describe= deserializar_paquete_describe(cliente_fd);

			if(paquete_describe != NULL)
			{

				if(strcmp(paquete_describe->nombre_tabla,"ALL") == 0){

					respuestaDESCRIBE* metadatasDeTablasPedidas = APIdescribeTodasLasTablasRESPUESTA();

					if(metadatasDeTablasPedidas->rta == 25){
						uint16_t rta=25;
						send(cliente_fd, &rta, sizeof(uint16_t), 0);

	//					imprimirListaMetadatas(metadatasDeTablasPedidas); SI DESCOMENTAS ESTO ROMPE PORQUE ES TIPO respuestaDESCRIBE*
						serealizar_respuesta_describe(metadatasDeTablasPedidas,cliente_fd);
					}
					if(metadatasDeTablasPedidas->rta == 23){
						uint16_t rta=23;
					send(cliente_fd, &rta, sizeof(uint16_t), 0);
					free(metadatasDeTablasPedidas);
					break;
					}
					//ME FALTA SI LA RTA ES ERRONEA.
					//dictionary_destroy_and_destroy_elements(metadatasDeTablasPedidas,free);

				} else{

					respuestaDESCRIBE* metadataDeTablaPedida = APIdescribeRESPUESTA(paquete_describe);
					if(metadataDeTablaPedida->rta == 22){
						uint16_t rta=22;
					send(cliente_fd, &rta, sizeof(uint16_t), 0);
					free(metadataDeTablaPedida);
					break;
					}

					if(metadataDeTablaPedida->rta == 20){
						uint16_t rta=20;
					send(cliente_fd, &rta, sizeof(uint16_t), 0);

	//				imprimirListaMetadatas(metadataDeTablaPedida->tablas);
					serealizar_respuesta_describe(metadataDeTablaPedida,cliente_fd);
					//dictionary_destroy_and_destroy_elements(metadataDeTablaPedida,free);
					}
				}
			} else
			{
				uint16_t rta=24;
				send(cliente_fd, &rta, sizeof(uint16_t), 0);
			}
			break;

		case SELECT:;

			t_paquete_select *paquete_select=deserializar_paquete_select(cliente_fd);

			if(paquete_select!=NULL){

				t_respuesta_pagina* rta_pagina= APIselectRESPUESTA(paquete_select,cliente_fd);

				int bytes = rta_pagina->tamanioPaquete;


				if(rta_pagina->bit != 1)
				{
					void * buffer = serializar_respuesta_pagina(rta_pagina);
					if (send(cliente_fd, &rta_pagina->tamanioPaquete, sizeof(size_t), MSG_DONTWAIT) <= 0)
						puts("Error en envio de CODIGO DE RESPUESTA.");

					if (send(cliente_fd, buffer, bytes, MSG_DONTWAIT) <= 0)
						puts("Error en envio de CODIGO DE RESPUESTA.");
					free(buffer);
				}
				else
				{
					uint16_t bit_error = 1;
					size_t tamanio_buffer = sizeof(uint16_t);

					send(cliente_fd,&tamanio_buffer,sizeof(size_t),0);
					send(cliente_fd,&bit_error,tamanio_buffer,0);
				}
				free(rta_pagina);

//				respuestaSELECT* rtaSELECT = malloc(sizeof(uint32_t) + sizeof(uint16_t));
//				rtaSELECT->keyHallada = malloc(1);
//				rtaSELECT->rta=34;
//				strcpy(rtaSELECT->keyHallada,"");
//				rtaSELECT->tamanio_key=1;
//
//				void* respuesta_a_enviar = serializar_respuesta_select(rtaSELECT);
//				if (send(cliente_fd, respuesta_a_enviar, sizeof(uint16_t)+ sizeof(uint32_t)+ rtaSELECT->tamanio_key, MSG_DONTWAIT) <= 0)
//						puts("Error en envio de CODIGO DE RESPUESTA.");
//				loggear_request_select(paquete_select);
			}



//			void* respuesta_a_enviar = serializar_respuesta_select(rtaSELECT);
//			if (send(cliente_fd, respuesta_a_enviar, sizeof(uint16_t)+ sizeof(uint32_t)+ rtaSELECT->tamanio_key, MSG_DONTWAIT) <= 0)
//				puts("Error en envio de CODIGO DE RESPUESTA.");

			break;

		case INSERT:;

			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);
			if(paquete_insert!=NULL){
//				rta= 43;
//				if (send(cliente_fd, &rta, sizeof(uint16_t), MSG_DONTWAIT) <= 0)
//					puts("Error en envio de CODIGO DE RESPUESTA.");
//				break;
				APIinsertRESPUESTA(paquete_insert,cliente_fd);
			}
//				rta= APIinsertRESPUESTA(paquete_insert);
//				if (send(cliente_fd, &rta, sizeof(uint16_t), MSG_DONTWAIT) <= 0)
//					puts("Error en envio de CODIGO DE RESPUESTA.");
			break;
		case JOURNAL:;

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

		//salgo mutex
		pthread_exit(NULL);
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

t_paquete_drop* deserializar_paquete_drop(int socket_cliente){

	size_t tamanioTabla;
	recv(socket_cliente, &tamanioTabla, sizeof(uint32_t) ,MSG_WAITALL);
	t_paquete_drop* paqueteDrop = malloc(tamanioTabla+sizeof(uint32_t));
	paqueteDrop->nombre_tabla = malloc(tamanioTabla);
	void *buffer = malloc(tamanioTabla);
	recv(socket_cliente, buffer, tamanioTabla ,MSG_WAITALL);
	memcpy(paqueteDrop->nombre_tabla,buffer, tamanioTabla);
	paqueteDrop->nombre_tabla_long=tamanioTabla;
	free(buffer);
	return paqueteDrop;

}

t_paquete_describe* deserializar_paquete_describe(int socket_cliente){

	size_t tamanioTabla;
	recv(socket_cliente, &tamanioTabla, sizeof(uint32_t) ,MSG_WAITALL);
	t_paquete_describe* paqueteDescribe= malloc(tamanioTabla+sizeof(uint32_t));
	paqueteDescribe->nombre_tabla = malloc(tamanioTabla);
	void *buffer = malloc(tamanioTabla);
	recv(socket_cliente, buffer, tamanioTabla ,MSG_WAITALL);
	memcpy(paqueteDescribe->nombre_tabla,buffer, tamanioTabla);
	paqueteDescribe->nombre_tabla_long=tamanioTabla;
	free(buffer);
	return paqueteDescribe;

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
	t_paquete_insert *paquete_insert = malloc(tamanioTabla + tamanioValue + sizeof(uint32_t)*2 + sizeof(uint16_t)+ sizeof(long long));

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
	t_paquete_create_de_mp* paquete_create = malloc(tamanio_tabla + tamanio_consistencia + sizeof(uint32_t)*2 + sizeof(int)*2);

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

void* serializar_respuesta_select(respuestaSELECT* respuestaSELECT){

		void* buffer = malloc(sizeof(uint32_t) + sizeof(uint16_t)  + respuestaSELECT->tamanio_key);
		int desplazamiento = 0;

		memcpy(buffer + desplazamiento, &respuestaSELECT->rta, sizeof(uint16_t));
		desplazamiento+= sizeof(uint16_t);

		memcpy(buffer + desplazamiento, &respuestaSELECT->tamanio_key, sizeof(uint32_t));
		desplazamiento+= sizeof(uint32_t);

		memcpy(buffer + desplazamiento, respuestaSELECT->keyHallada, respuestaSELECT->tamanio_key);
		desplazamiento+= respuestaSELECT->tamanio_key;

		return buffer;
}

t_paquete_create* adaptadorDePaquete(t_paquete_create_de_mp* paquete_create_mp){
	t_paquete_create* paquete_adaptado = crear_paquete_create(
			paquete_create_mp->nombre_tabla,
			pasarAConsistenciaINT(paquete_create_mp->consistencia),
			paquete_create_mp->particiones,
			paquete_create_mp->tiempo_compactacion);
	//printf("\nEL TIEMPO DE COMPACTACION ES: %d\n",paquete_create_mp->tiempo_compactacion);

//	memcpy(paquete_adaptado->nombre_tabla,paquete_create_mp->nombre_tabla,sizeof(paquete_create_mp->nombre_tabla));
//	paquete_adaptado->metadata.particiones = paquete_create_mp->particiones;
//	paquete_adaptado->metadata.tiempo_de_compactacion = paquete_create_mp->tiempo_compactacion;
//	paquete_adaptado->metadata.consistencia = pasarAConsistenciaINT(paquete_create_mp->consistencia);

	return paquete_adaptado;
}


void serealizar_respuesta_describe(respuestaDESCRIBE* metadatasDeTablasPedidas,int conexion){
	void _pasarAvaloresDescribe(char* key, void* metadata){
		pasarAvaloresDescribe(key,  metadata,conexion);
	}

	int cantidadDeTablas= dictionary_size(metadatasDeTablasPedidas->tablas);
	int i =0;
	send(conexion, &cantidadDeTablas, sizeof(int), 0);
	dictionary_iterator(metadatasDeTablasPedidas->tablas,_pasarAvaloresDescribe);
 }


void pasarAvaloresDescribe(char* key, void* metadata,int conexion){

	t_metadata_fs* metadataTabla = metadata;

	int longNombreTabla= strlen(key)+1;
	int longConsistencia=strlen(pasarAConsistenciaChar(metadataTabla->consistencia))+1; //AGREGÉ UN +1

	void* buffer=malloc(longNombreTabla+longConsistencia +sizeof(int)*4);

	int desplazamiento=0;
	memcpy(buffer+desplazamiento, &longNombreTabla,sizeof(int));
	desplazamiento+=sizeof(int);
	memcpy(buffer+desplazamiento, &longConsistencia,sizeof(int));
	desplazamiento+=sizeof(int);
	memcpy(buffer+desplazamiento, key,longNombreTabla);
	desplazamiento+=longNombreTabla;
	memcpy(buffer+desplazamiento, pasarAConsistenciaChar(metadataTabla->consistencia),longConsistencia);
	desplazamiento+=longConsistencia;

	memcpy(buffer+desplazamiento,&metadataTabla->particiones,sizeof(int));
	desplazamiento+=sizeof(int);
	memcpy(buffer+desplazamiento,&metadataTabla->tiempo_de_compactacion,sizeof(int));
	desplazamiento+=sizeof(int);

	send(conexion, buffer, longNombreTabla+longConsistencia +sizeof(int)*4, 0);



}










