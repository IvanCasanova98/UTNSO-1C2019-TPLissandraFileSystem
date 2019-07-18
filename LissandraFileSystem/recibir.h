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

typedef struct t_paquete_create_de_mp
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	char* nombre_tabla;
	uint32_t consistencia_long;
	char* consistencia;//CREAR TIPO DE DATO
	int particiones;
	int tiempo_compactacion;
}t_paquete_create_de_mp;

typedef struct respuestaSELECT
{
	uint32_t tamanio_key;
	uint16_t rta;
	char* keyHallada;
}respuestaSELECT;

typedef struct respuestaDESCRIBE
{
	uint16_t rta;
	t_dictionary* tablas;
}respuestaDESCRIBE;

typedef struct valoresDescribe
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	char* nombre_tabla;
	t_metadata metadata;

}valoresDescribe;


void* pasarAvaloresDescribe(char* key, void* metadata,t_list* lista);
t_list* convertirDiccionaryALista(t_dictionary* diccionario);
void* serealizar_respuesta_describe(respuestaDESCRIBE* metadatasDeTablasPedidas);
//--------------------RECIBIR PAQUETE
void* recibir_paquetes(void*);

//--------------------RECIBIR OPERACION
int recibir_operacion(int);

//---------------------DESERIALIZAR PAQUETE
t_paquete_select* deserializar_paquete_select(int socket_cliente);
t_paquete_insert* deserializar_paquete_insert(int socket_cliente);
t_paquete_create_de_mp* deserializar_paquete_create_de_mp(int socket_cliente);
t_paquete_drop* deserializar_paquete_drop(int socket_cliente);
t_paquete_describe* deserializar_paquete_describe(int socket_cliente);

void* serializar_respuesta_select(respuestaSELECT* respuestaSELECT);
//--------------------ADAPTADOR
t_paquete_create* adaptadorDePaquete(t_paquete_create_de_mp* paquete_create_mp);

//--------------------ARCHIVOS LOGGER/CONFIGURACION
t_log* iniciar_logger(void);
t_config* leer_config(void);

//--------------------ARCHIVO CONEXION.C
int esperar_cliente(int);

#endif /* RECIBIR_H_ */
