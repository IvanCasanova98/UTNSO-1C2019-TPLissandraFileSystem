#ifndef UTILS_H_
#define UTILS_H_

#include <commons/log.h>
#include <commons/config.h>

typedef struct SEED
{
	char* IP;
	char* PUERTO;
}SEED;

SEED seed[];

//----------------------------ARCHIVOS LOGGER/CONFIG
t_log* iniciar_logger(void);
t_config* leer_config(void);
//--------------------------------------------------
int codigo_ingresado(char* parametros);
int numero_random(int max);

#endif /* UTILS_H_ */
