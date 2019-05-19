#ifndef PAQUETE_H_
#define PAQUETE_H_

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
	ADD
}op_code;

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
	int timestamp;
}t_paquete_insert;

//---------------------CREAR PAQUETE
t_paquete_select* crear_paquete_select(char *nombretabla,uint16_t valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,uint16_t valor_key, char *value, long long timestamp);

//---------------------ARMAR PAQUETE
t_paquete_select* paquete_select(char* parametros, t_log* logger);
t_paquete_insert* paquete_insert(char* parametros, t_log* logger);

//---------------------LOGGEAR PAQUETE (funciones de utils.c)
void loggear_paquete_select(t_paquete_select* paquete, t_log* logger);
void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger);

#endif /* PAQUETE_H_ */
