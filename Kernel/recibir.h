#ifndef RECIBIR_H_
#define RECIBIR_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

#include<sys/socket.h>
#include<commons/string.h>

#include "utils.h"

typedef struct{
	char* nombre_tabla;
	char* consistencia;
}t_metadata;


void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(int socket_cliente);
void recibir_seed(int socket_cliente);
t_metadata* deserealizar_nodo(int socket);

#endif /* RECIBIR_H_ */