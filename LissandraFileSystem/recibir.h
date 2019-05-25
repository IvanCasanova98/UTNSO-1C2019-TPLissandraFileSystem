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
#include<pthread.h>

t_log* logger;

typedef enum op_code
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

typedef enum consistency
{
	SC,
	SHC,
	EC

}consistency;


typedef struct  argumentosEnvioPaquete{
	int clientefd;
	int serverfd;

}argumentosEnvioPaquete;

typedef struct t_metadata
{
	consistency consistencia;
	int particiones;
	int tiempo_de_compactacion;
}t_metadata;

typedef struct t_registro
{
	char* value;
	uint16_t key;
	long long timestamp;

}t_registro;

typedef struct t_paquete_select
{
	char* nombre_tabla;
	uint32_t nombre_tabla_long;
	uint16_t valor_key;
}t_paquete_select;

typedef struct t_paquete_drop{
	uint32_t nombre_tabla_long;
	char* nombre_tabla;
} t_paquete_drop;

typedef struct t_paquete_describe{
	uint32_t nombre_tabla_long;
	char* nombre_tabla;
} t_paquete_describe;

typedef struct t_paquete_insert
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	uint32_t value_long; 		//Longitud del value
	char* nombre_tabla;
	uint16_t valor_key;
	char* value;
	long long timestamp;
}t_paquete_insert;

typedef struct t_paquete_create
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	char* nombre_tabla;
	t_metadata metadata;

}t_paquete_create;

//--------------------RECIBIR PAQUETE
void* recibir_paquetes(void*);

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
