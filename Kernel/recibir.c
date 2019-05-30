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
	printf("\nValue Recibido:  %s", buffer);
	free(buffer);
}

void recibir_seed(int socket_cliente)
{
	int size1, i=0, size2, j=0;

	int cant;
	recv(socket_cliente, &cant, sizeof(int), 0);

	SEED seed_aux[cant];

	for(int i=0; i<cant; i++)
	{
		seed[i].IP = seed_aux[i].IP;
		seed[i].PUERTO = seed_aux[i].PUERTO;
	}

	while(i<cant)//Recibe IPS
	{
		char* array_IP_SEED = recibir_buffer(&size1, socket_cliente);

		seed[i].IP = array_IP_SEED;

		i++;
	}

	while(j<cant) //Recibe PUERTOS
	{
		char* array_PUERTO_SEED = recibir_buffer(&size2, socket_cliente);

		seed[j].PUERTO = array_PUERTO_SEED;

		j++;
	}
}
