#ifndef CONEXION_H_
#define CONEXION_H_

#include<netdb.h>
#include<string.h>
#include<unistd.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

#include "paquete.h"
#include "recibir.h"
#include "utils.h"

int crear_conexion(char *ip, char* puerto);
int iniciar_conexion_inicial(t_log* logger, char* ip, char* puerto);
int iniciar_conexion_request(t_log* logger, char* ip, char* puerto);
int conectarse_a_memoria(char** vector_request, t_log* logger);
void pedir_seed(int conexion);
void terminar_kernel(t_log* logger, t_config* config);

#endif /* CONEXION_H_ */
