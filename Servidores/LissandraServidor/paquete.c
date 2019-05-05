#include "paquete.h"

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

t_paquete_select* deserializar_paquete_select(int socket_cliente){


	int desplazamiento = 0;
	size_t tamanioTabla;
	recv(socket_cliente, &tamanioTabla, 4 ,MSG_WAITALL);
	t_paquete_select *paqueteSelect= malloc(tamanioTabla+8);
	paqueteSelect->nombre_tabla = malloc(tamanioTabla);
	paqueteSelect->nombre_tabla_long = malloc(4);
	paqueteSelect->valor_key = malloc(4);

	void *buffer = malloc(tamanioTabla+sizeof(int));

	recv(socket_cliente, buffer, tamanioTabla+4 ,MSG_WAITALL);
	memcpy(paqueteSelect->nombre_tabla,buffer + desplazamiento, tamanioTabla);
	printf("%s",paqueteSelect->nombre_tabla);
	desplazamiento+= tamanioTabla;
	memcpy(&(paqueteSelect->valor_key),buffer + desplazamiento, sizeof(int));
	desplazamiento+= sizeof(int);
	paqueteSelect->nombre_tabla_long=tamanioTabla;
	free(buffer);
	return paqueteSelect;
}


t_paquete_insert* deserializar_paquete_insert(int socket_cliente){

	void *buffer_para_longitudes = malloc(sizeof(uint32_t)*2); //hice este buffer para sacar los tamaños de las longitudes variables. EL otro buffer saca TODa la info deaa
	int desplazamiento = 0;
	size_t tamanioTabla;
	size_t tamanioValue;
	recv(socket_cliente, buffer_para_longitudes, sizeof(uint32_t)*2 ,MSG_WAITALL);

	memcpy(&tamanioTabla ,buffer_para_longitudes,sizeof(uint32_t));
	memcpy(&tamanioValue ,buffer_para_longitudes+sizeof(uint32_t),sizeof(uint32_t));
//	t_paquete_insert *paquete_insert = malloc(tamanioTabla + tamanioValue + sizeof(int)*2 + sizeof(uint32_t)*2);
	t_paquete_insert *paquete_insert = malloc(tamanioTabla + tamanioValue + 16);

		paquete_insert->nombre_tabla = malloc(tamanioTabla);
		paquete_insert->value = malloc(tamanioValue);
		paquete_insert->valor_key = malloc(4);
		paquete_insert->timestamp =malloc(4);


	void *buffer = malloc(8 +tamanioTabla+tamanioValue);
	recv(socket_cliente, buffer, 8 +tamanioTabla+tamanioValue ,MSG_WAITALL);

	memcpy(paquete_insert->nombre_tabla,buffer + desplazamiento,tamanioTabla);
	desplazamiento+= tamanioTabla;
	memcpy(paquete_insert->value,buffer + desplazamiento, tamanioValue);
	desplazamiento+= tamanioValue;
	memcpy(&(paquete_insert->valor_key),buffer + desplazamiento, sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(&(paquete_insert->timestamp),buffer + desplazamiento, sizeof(int));
	desplazamiento+= sizeof(int);
	paquete_insert->nombre_tabla_long = tamanioTabla;
	paquete_insert->value_long = tamanioValue;
	free(buffer_para_longitudes);
	free(buffer);
	return paquete_insert;
}



void loggear_paquete_select(t_paquete_select* paquete_select){

	t_log* logger = iniciar_logger();
	log_info(logger, "Se recibio SELECT %s %d \n",paquete_select->nombre_tabla, paquete_select->valor_key);
	log_destroy(logger);
}

void loggear_paquete_insert(t_paquete_insert* paquete_insert){

	t_log* logger = iniciar_logger();
	log_info(logger, "Se recibio paquete tipo: INSERT %s %d %s %d \n",paquete_insert->nombre_tabla, paquete_insert->valor_key,paquete_insert->value, paquete_insert->timestamp);
	log_destroy(logger);
}

t_log* iniciar_logger() {
	return log_create("LissandraFileSystem.log", "LissandraFileSystem", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	return config_create("LissandraFileSystem.config");
}
