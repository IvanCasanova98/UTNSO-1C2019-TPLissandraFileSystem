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

void* recibir_buffer(int*, int);
int iniciar_servidor(char *ip, char* puerto);
int esperar_cliente(int);

#endif /* CONEXIONES_H_ */
