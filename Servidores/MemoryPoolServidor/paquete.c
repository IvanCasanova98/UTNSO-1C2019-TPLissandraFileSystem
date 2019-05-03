#include "paquete.h"

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
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
	printf("%d",tamanioTabla);
	t_paquete_select *paqueteSelect= malloc(tamanioTabla+8);
	paqueteSelect->nombre_tabla = malloc(tamanioTabla);
	paqueteSelect->nombre_tabla_long = malloc(4);
	paqueteSelect->valor_key = malloc(4);

	void *buffer = malloc(tamanioTabla+4);

	recv(socket_cliente, buffer, tamanioTabla+4 ,MSG_WAITALL);
	memcpy(paqueteSelect->nombre_tabla,buffer + desplazamiento, tamanioTabla);
	printf("%s",paqueteSelect->nombre_tabla);
	desplazamiento+= tamanioTabla;
	memcpy(&(paqueteSelect->valor_key),buffer + desplazamiento, 4);
	desplazamiento+= 4;
	paqueteSelect->nombre_tabla_long=tamanioTabla;
	free(buffer);
	return paqueteSelect;
}


t_paquete_insert* deserializar_paquete_insert(int socket_cliente){


	int desplazamiento = 0;
	void *buffer = malloc(35);
	t_paquete_insert *paqueteinsert = malloc(35);
	recv(socket_cliente, buffer, 35 ,MSG_WAITALL);
	memcpy(&(paqueteinsert->nombre_tabla),buffer + desplazamiento,7);
	desplazamiento+= 7;
	memcpy(&(paqueteinsert->valor_key),buffer + desplazamiento, 4);
	desplazamiento+= 4;
	memcpy(&(paqueteinsert->value),buffer + desplazamiento, 20);
	desplazamiento+= 20;
	memcpy(&(paqueteinsert->timestamp),buffer + desplazamiento, 4);
	desplazamiento+= 4;
	free(buffer);
	return paqueteinsert;
}



void loggear_paquete_select(t_paquete_select* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "\nNuevo paquete creado\nTipo paquete: SELECT\nNombre tabla: %s\nValor KEY: %d", paquete->nombre_tabla,paquete->valor_key);
    log_destroy(logger);
}

void loggear_paquete_insert(t_paquete_insert* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "\nNuevo paquete creado\nTipo paquete: INSERT\nNombre tabla: %s\nValor KEY: %d\nValue del paquete: %s\nTimestamp: %d", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
    log_destroy(logger);
}

