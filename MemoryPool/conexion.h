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

//--------------------LEVANTAR SERVIDOR
void* servidor(void *arg);

//----------------------------CONEXION COMO SERVIDOR
int iniciar_servidor();
int esperar_cliente(int);

//----------------------------CONEXION COMO CLIENTE
int iniciar_conexion(t_log* logger, t_config* config);
int crear_conexion(char *ip, char* puerto);
void terminar_conexion(t_log* logger, t_config* config, int conexion);

#endif /* CONEXION_H_ */
