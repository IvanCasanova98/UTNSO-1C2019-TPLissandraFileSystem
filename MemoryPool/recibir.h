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

typedef enum
{
	CREATE,
	DROP,
	DESCRIBE,
	SELECT,
	INSERT,
	JOURNAL,
	RUN,
	ADD
}op_code;

typedef struct t_paquete_select
{
	char* nombre_tabla;
	uint32_t nombre_tabla_long;
	int valor_key;
}t_paquete_select;

typedef struct t_paquete_insert
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	uint32_t value_long; 		//Longitud del value
	char* nombre_tabla;
	int valor_key;
	char* value;
	uint16_t timestamp;
}t_paquete_insert;

//--------------------RECIBIR PAQUETE
void recibir_paquetes(t_log* logger, int cliente_fd, int server_fd);

//--------------------RECIBIR OPERACION
int recibir_operacion(int);

//---------------------DESERIALIZAR PAQUETE
t_paquete_select* deserializar_paquete_select(int socket_cliente);
t_paquete_insert* deserializar_paquete_insert(int socket_cliente);

//--------------------ARCHIVOS LOGGER/CONFIGURACION
t_log* iniciar_logger(void);
t_config* leer_config(void);

//--------------------ARCHIVO CONEXION.C
int esperar_cliente(int);

#endif /* RECIBIR_H_ */
