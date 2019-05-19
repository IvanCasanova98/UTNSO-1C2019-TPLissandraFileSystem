#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<commons/string.h>
#include<commons/config.h>

#include "paquete.h"

struct parametros
{
	t_config* config;
	t_log* logger;
};

//----------------------------ARCHIVOS LOGGER/CONFIG
t_log* iniciar_logger(void);
t_config* leer_config(char* archivo);

//---------------------LOGGEAR PAQUETE
void loggear_paquete_select(t_paquete_select* paquete, t_log* logger);
void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger);

#endif /* UTILS_H_ */
