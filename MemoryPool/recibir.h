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

//--------------------RECIBIR PAQUETE
void recibir_paquetes(t_log* logger, int cliente_fd, int server_fd);

//--------------------RECIBIR OPERACION
int recibir_operacion(int);

//---------------------DESERIALIZAR PAQUETE
t_paquete_select* deserializar_paquete_select(int socket_cliente);
t_paquete_insert* deserializar_paquete_insert(int socket_cliente);

//--------------------ARCHIVO CONEXION.C
int esperar_cliente(int);

#endif /* RECIBIR_H_ */
