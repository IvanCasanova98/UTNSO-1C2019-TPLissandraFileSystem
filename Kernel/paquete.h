#ifndef PAQUETE_H_
#define PAQUETE_H_

#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>



//---------------------ESTRUCTURA PAQUETES

typedef struct
{
	int size;
	void* stream;
} t_buffer;

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

typedef struct
{
	op_code codigo_operacion;
	char nombre_tabla[20];
	int valor_key;
	t_buffer* buffer;
} t_paquete_select;

typedef struct
{
	op_code codigo_operacion;
	char nombre_tabla[20];
	int valor_key;
	char value[20];
	int timestamp;
	t_buffer* buffer;
} t_paquete_insert;

t_log* iniciar_logger(void);
t_config* leer_config(void);

//---------------------CREAR PAQUETE

t_paquete_select* crear_paquete_select(char *nombretabla,int valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp);

//---------------------ARMAR PAQUETE

t_paquete_select* selectf();
t_paquete_insert* insert();

//---------------------LOGGEAR PAQUETE

void loggear_paquete_select(t_paquete_select* paquete);
void loggear_paquete_insert(t_paquete_insert* paquete);

//----------------------------------------------------ELIMINAR PAQUETE

void eliminar_paquete_select(t_paquete_select* paquete);
void eliminar_paquete_insert(t_paquete_insert* paquete);

#endif /* PAQUETE_H_ */