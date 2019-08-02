#ifndef RECIBIR_H_
#define RECIBIR_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

#include<sys/socket.h>
#include<commons/string.h>

#include "criterio.h"
#include "utils.h"

typedef struct{
	char* nombre_tabla;
	char* consistencia;
}t_metadata;


void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(int socket_cliente);
void recibir_seed(int socket_cliente);
void _eliminar_seed(SEED* memoria);
t_metadata* deserealizar_nodo(int socket);

bool table_has_memory(int numero_memoria);

SEED* deserealizar_seed(int socket_cliente);

#endif /* RECIBIR_H_ */
