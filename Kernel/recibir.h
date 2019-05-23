#ifndef RECIBIR_H_
#define RECIBIR_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

#include<sys/socket.h>
#include<commons/string.h>

void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(int socket_cliente);

#endif /* RECIBIR_H_ */
