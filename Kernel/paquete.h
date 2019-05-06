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
	ADD
}op_code; //TIPO DE PAQUETE

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
	int valor_key;
	uint32_t value_long; 		//Longitud del value
	char* value;
	int timestamp;
}__attribute__((packed)) t_paquete_insert;

//---------------------ARCHIVO LOGGER/CONFIG
t_log* iniciar_logger(void);
t_config* leer_config(void);

//----------------------------ENVIAR PAQUETES
void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente);

//---------------------INGRESO DE PAQUETE(FUNC AUX)
char* ingresar_request();
int codigo_ingresado(char* lineaRequest);

//---------------------CREAR PAQUETE
t_paquete_select* crear_paquete_select(char *nombretabla,int valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp);

//---------------------ARMAR PAQUETE
t_paquete_select* selectf(char* parametros);
t_paquete_insert* insert(char* parametros);

//---------------------LOGGEAR PAQUETE
void loggear_paquete_select(t_paquete_select* paquete);
void loggear_paquete_insert(t_paquete_insert* paquete);

//----------------------------------------------------ELIMINAR PAQUETE
//void eliminar_paquete_select(t_paquete_select* paquete);
//void eliminar_paquete_insert(t_paquete_insert* paquete);

#endif /* PAQUETE_H_ */
