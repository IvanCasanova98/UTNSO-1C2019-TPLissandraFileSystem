#ifndef RECIBIR_H_
#define RECIBIR_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include<commons/config.h>
#include<string.h>

#include "paquete.h"
#include "API.h"
#include "utils.h"

//--------------------LEVANTAR SERVIDOR
void* servidor(void * arg);

//--------------------RECIBIR PAQUETE
void recibir_paquetes(int cliente_fd, int server_fd, t_config* config, t_log* logger);

//--------------------RECIBIR OPERACION
int recibir_operacion(int);

//---------------------DESERIALIZAR PAQUETE
t_paquete_select* deserializar_paquete_select(int socket_cliente);
t_paquete_insert* deserializar_paquete_insert(int socket_cliente);

//--------------------ARCHIVO CONEXION.C
int esperar_cliente(int); //conexion.c ya incluye a "recibir.h", por lo que no puedo agregar a "conexion.h" en este header

#endif /* RECIBIR_H_ */
