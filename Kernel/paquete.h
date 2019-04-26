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
	char valor_key[20];
	t_buffer* buffer;
} t_paqueteSELECT;

typedef struct
{
	op_code codigo_operacion;
	char nombre_tabla[20];
	char valor_key[20];
	char value[20];
	char timestamp[20];
	t_buffer* buffer;
} t_paqueteINSERT;

t_log* iniciar_logger(void);
t_config* leer_config(void);

//---------------------CREAR PAQUETE

t_paqueteSELECT* crear_paqueteSELECT();
t_paqueteINSERT* crear_paqueteINSERT();

//---------------------ARMAR PAQUETE

t_paqueteSELECT* selectf();
t_paqueteINSERT* insert();

//---------------------LOGGEAR PAQUETE

void loggear_paqueteSELECT(t_paqueteSELECT* paquete);
void loggear_paqueteINSERT(t_paqueteINSERT* paquete);


#endif /* PAQUETE_H_ */
