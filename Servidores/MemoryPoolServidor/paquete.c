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

t_paquete_select* crear_paquete_select(char* nombretabla,int valor_key) //Agregado
{
	t_paquete_select* paquete = malloc(11);
	for(int i=0;i<7;i++){
		paquete->nombre_tabla[i]= nombretabla[i];
	}
	paquete->valor_key = valor_key;

	return paquete;

}

t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp) //Agregado
{
	t_paquete_insert* paquete = malloc(35);

	for(int i=0;i<7;i++){
		paquete->nombre_tabla[i]= nombretabla[i];
	}
	for(int i=0;i<20;i++){
		paquete->value[i] = value[i];
	}
	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;


	return paquete;
}


t_paquete_select* deserializar_paquete_select(int socket_cliente){


	int desplazamiento = 0;
	void *buffer = malloc(11);
	struct t_paquete_select *paqueteSelect = malloc(11);
	recv(socket_cliente, buffer, 11 ,MSG_WAITALL);
	memcpy(&(paqueteSelect->nombre_tabla),buffer + desplazamiento,7);
	desplazamiento+= 7;
	memcpy(&(paqueteSelect->valor_key),buffer + desplazamiento, 4);
	desplazamiento+= 4;
	free(buffer);
	return paqueteSelect;
}


t_paquete_insert* deserializar_paquete_insert(int socket_cliente){


	int desplazamiento = 0;
	void *buffer = malloc(35);
	struct t_paquete_insert *paqueteinsert = malloc(35);
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

