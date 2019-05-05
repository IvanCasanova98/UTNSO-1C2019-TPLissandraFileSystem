#ifndef CONEXIONES_H_
#define CONEXIONES_H_

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



typedef enum
{
	CREATE, //Agregado
	DROP, //Agregado
	DESCRIBE, //Agregado
	SELECT, //Agregado
	INSERT, //Agregado
	JOURNAL, //Agregado
	RUN, //Agregado
	ADD //Agregado
}op_code; //TIPO DE PAQUETE

t_log* logger;

int iniciar_conexion(t_log* logger, t_config* config);
void* recibir_buffer(int*, int);
void terminar_conexion(t_log* logger, t_config* config, int conexion);
int iniciar_servidor(char *ip, char* puerto);
int esperar_cliente(int);
int crear_conexion(char *ip, char* puerto);

#endif /* CONEXIONES_H_ */
