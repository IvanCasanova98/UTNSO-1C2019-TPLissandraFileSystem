#ifndef PAQUETE_H_
#define PAQUETE_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

#include<sys/socket.h>

//#include<readline/readline.h>
//#include<readline/history.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include <sys/time.h>

#include"recibir.h"
#include"criterio.h"
#include "memorias.h"
#include "utils.h"

//---------------------ESTRUCTURA PAQUETES
typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef enum
{
	CREATE,
	DROP,
	DESCRIBE,
	SELECT,
	INSERT,
	JOURNAL,
	RUN,
	ADD,
	HS //HandShake
}op_code; //TIPO DE PAQUETE

typedef struct t_paquete_create
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	char* nombre_tabla;
	uint32_t consistencia_long;
	char* consistencia;//CREAR TIPO DE DATO
	int particiones;
	int tiempo_compactacion;
}__attribute__((packed)) t_paquete_create;

typedef struct t_paquete_select
{
	uint32_t nombre_tabla_long;
	char* nombre_tabla;
	int valor_key;

}__attribute__((packed)) t_paquete_select;

typedef struct t_paquete_insert
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	char* nombre_tabla;
	uint16_t valor_key;
	uint32_t value_long; 		//Longitud del value
	char* value;
	long long timestamp;
}__attribute__((packed)) t_paquete_insert;

typedef struct t_paquete_drop{
	uint32_t nombre_tabla_long;
	char* nombre_tabla;
} t_paquete_drop;

//---------------------ARCHIVO LOGGER/CONFIG
t_log* iniciar_logger(void);
t_config* leer_config(void);

//----------------------------ENVIAR PAQUETES
void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente);

//---------------------CREAR PAQUETE
t_paquete_select* crear_paquete_select(char *nombretabla,uint16_t valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,uint16_t valor_key, char *value, long long timestamp);
t_paquete_create* crear_paquete_create(char* nombre_tabla, char* consistencia, int particiones, int tiempo_compactacion);
void describe(int conexion, char* parametros);

//---------------------ARMAR PAQUETE
t_paquete_select* selectf(char** vector_parametros);
t_paquete_insert* insert(char** vector_parametros);
long long get_timestamp();
t_paquete_create* create(char** vector_parametros);

//---------------------LOGGEAR PAQUETE
void loggear_paquete_select(t_paquete_select* paquete);
void loggear_paquete_insert(t_paquete_insert* paquete);
void loggear_paquete_create(t_paquete_create* paquete);

void logger_describe_tabla(char* nombre_tabla);
void logger_describe_all();

//----------------------------------------------------ELIMINAR PAQUETE
//void eliminar_paquete_select(t_paquete_select* paquete);
//void eliminar_paquete_insert(t_paquete_insert* paquete);

#endif /* PAQUETE_H_ */
