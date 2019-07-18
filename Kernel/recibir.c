#include "recibir.h"

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), 0);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, 0);

	return buffer;
//	free(buffer);
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

void recibir_seed(int socket_cliente)
{
	int size1, i=0, size2, j=0;
	int cant;
	recv(socket_cliente, &cant, sizeof(int), 0);

	printf("\nCANTIDAD RECIBIDA:%d\n");
	SEED seed_aux[cant];

	for(int i=0; i<cant; i++)
	{
		seed[i].IP = seed_aux[i].IP;
		seed[i].PUERTO = seed_aux[i].PUERTO;
	}

	while(i<=2)//Recibe IPS
	{
		char* array_IP_SEED = recibir_buffer(&size1, socket_cliente);

		seed[i].IP = array_IP_SEED;

		i++;
	}

	while(j<=2) //Recibe PUERTOS
	{
		char* array_PUERTO_SEED = recibir_buffer(&size2, socket_cliente);

		seed[j].PUERTO = array_PUERTO_SEED;

		j++;
	}


	for(int indiceIP=0; indiceIP<cant; indiceIP++){
		printf("\n%s\n",seed[indiceIP].IP);
	}
	for(int indicePUERTO=0; indicePUERTO<cant; indicePUERTO++){
		printf("\n%s\n",seed[indicePUERTO].PUERTO);
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
