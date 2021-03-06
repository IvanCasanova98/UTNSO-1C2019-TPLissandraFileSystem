#ifndef PAQUETE_H_
#define PAQUETE_H_

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

#include<sys/socket.h>

#include<readline/readline.h>
#include<readline/history.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include <sys/time.h>

//#include "utils.h"

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct SEED
{
	char* IP;
	int PUERTO;
	int NUMBER;
}SEED;


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
	HS, //HandShake
	GOSSIPING
}op_code;

typedef enum consistency
{
	SC,
	SHC,
	EC

}consistency;

typedef struct t_paquete_describe_lfs{
	uint32_t nombre_tabla_long;
	char* nombre_tabla;
} t_paquete_describe_lfs;

typedef struct t_metadata_fs
{
	consistency consistencia;
	int particiones;
	int tiempo_de_compactacion;
}t_metadata_fs;

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
	char* nombre_tabla;
	uint32_t nombre_tabla_long;
	uint16_t valor_key;
}t_paquete_select;

typedef struct t_paquete_insert
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	uint32_t value_long; 		//Longitud del value
	char* nombre_tabla;
	uint16_t valor_key;
	char* value;
	long long timestamp;
}t_paquete_insert;



//---------------------CREAR PAQUETE
t_paquete_select* crear_paquete_select(char *nombretabla,uint16_t valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,uint16_t valor_key, char *value, long long timestamp);
t_paquete_create* crear_paquete_create(char* nombre_tabla, char* consistencia, int particiones, int tiempo_compactacion);
t_paquete_describe_lfs* paquete_describe_para_lfs(char* parametros,t_log* logger);
//---------------------ARMAR PAQUETE

t_paquete_select* paquete_select(char* parametros, t_log* logger);
t_paquete_insert* paquete_insert(char* parametros, t_log* logger);


long long get_timestamp(char * parametros);
t_paquete_create* paquete_create(char* parametros, t_log* logger);
void describe(int conexion, char* parametros);
//---------------------LOGGEAR PAQUETE (funciones de utils.c)
void loggear_paquete_select(t_paquete_select* paquete, t_log* logger);
void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger);
void loggear_paquete_create(t_paquete_create* paquete, t_log* logger);

//------------------

bool validar_numero(char* parametro);

bool validarConsistencia(char* consistencia);


#endif /* PAQUETE_H_ */
