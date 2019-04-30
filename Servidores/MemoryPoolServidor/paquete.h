#ifndef PAQUETE_H_
#define PAQUETE_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

//---func prueba
void nombre_tabla(int socket_cliente);
int valor_key(int socket_cliente);

#endif /* PAQUETE_H_ */
