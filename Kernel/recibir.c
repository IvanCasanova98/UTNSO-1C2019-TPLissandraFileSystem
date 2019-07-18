#include "recibir.h"

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

SEED* deserealizar_seed(int socket_cliente)
{


	int tamanio_ip;
	recv(socket_cliente, &tamanio_ip, sizeof(int), 0);


	int size=tamanio_ip+2*sizeof(int);
	void * buffer = malloc(size);

	recv(socket_cliente, buffer, size, 0);

	int desplazamiento = 0;

	SEED* aux = malloc(size);
	aux->IP = malloc(tamanio_ip);

	memcpy(aux->IP,buffer + desplazamiento, tamanio_ip);
	desplazamiento+=tamanio_ip;

	memcpy(&(aux->NUMBER),buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(int);

	memcpy(&(aux->PUERTO),buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(int);


	return aux;
}


void recibir_seed(int socket_cliente)
{
	int size1, i=0, j=0, k=0;

	int cant;
	recv(socket_cliente, &cant, sizeof(int), 0);

	while(i<cant){
		SEED *memoria_i = deserealizar_seed(socket_cliente);
		list_add(lista_seeds,memoria_i);
		i++;
	//printf("\nMemoria: %d - Puerto: %d - Ip: %s", memoria_i->NUMBER,memoria_i->PUERTO,memoria_i->IP);
	}
}

t_metadata* deserealizar_nodo(int socket){

	int desplazamiento = 0;
	int tamanio_total = 0;
	int nombre_tabla_long = 0;
	int consistencia_long = 0;

	recv(socket, &tamanio_total, sizeof(int), MSG_WAITALL);

	t_metadata* nodo_metadata = malloc(tamanio_total - (2*sizeof(int)));

	void* buffer = malloc(tamanio_total);

	recv(socket, buffer, tamanio_total, MSG_WAITALL);

	memcpy(&(nombre_tabla_long), buffer + desplazamiento,sizeof(int));
	desplazamiento+=sizeof(int);

	nodo_metadata->nombre_tabla = malloc(nombre_tabla_long);

	memcpy(nodo_metadata->nombre_tabla, buffer+desplazamiento,nombre_tabla_long);
	desplazamiento+=nombre_tabla_long;

	memcpy(&(consistencia_long), buffer + desplazamiento,sizeof(int));
	desplazamiento+=sizeof(int);

	nodo_metadata->consistencia = malloc(consistencia_long);

	memcpy(nodo_metadata->consistencia, buffer+desplazamiento,consistencia_long);
	desplazamiento+=consistencia_long;

	free(buffer);
	return nodo_metadata;
	free(nodo_metadata);

}
