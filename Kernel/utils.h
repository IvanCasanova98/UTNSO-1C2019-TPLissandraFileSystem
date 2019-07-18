#ifndef UTILS_H_
#define UTILS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <stdio.h>
#include <unistd.h>

typedef enum
{
	SC,
	SHC,
	EC
};

typedef struct SEED
{
	char* IP;
	int PUERTO;
	int NUMBER;
}SEED;



struct parametros
{
	int conexion;
	char* parametros;
	t_config* config;
	t_log* logger;
};

//----------------------------ARCHIVOS LOGGER/CONFIG
t_log* iniciar_logger(void);
t_config* leer_config(void);
//--------------------------------------------------
int codigo_ingresado(char* parametros);
int consistencia_ingresada(char* parametros);
int numero_random(int max);
//--------------------------------------------------
void retardo_ejecucion(t_config* config);

char * concatenar_value(char ** vector);

//---------------------------VALIDACIONES
bool validarNumero(char* parametro);
void falta_tabla();
void falta_key();
void falta_value();
void falta_consistencia();
void falta_particiones();
void falta_tiempo_compactacion();
void describe_invalido();


#endif /* UTILS_H_ */
