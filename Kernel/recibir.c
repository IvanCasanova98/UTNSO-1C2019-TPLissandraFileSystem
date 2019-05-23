#include "recibir.h"

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), 0);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, 0);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	printf("\n Value Recibido:  %s", buffer);
	free(buffer);
}
