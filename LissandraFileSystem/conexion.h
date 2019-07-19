#ifndef CONEXION_H_
#define CONEXION_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<commons/string.h>
#include<commons/config.h>

#include "recibir.h"

t_log* logger;

void* hiloEscucha();
int iniciar_servidor();
int esperar_cliente(int);

#endif /* CONEXION_H_ */
