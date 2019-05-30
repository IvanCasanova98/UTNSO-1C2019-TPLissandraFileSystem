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

int iniciar_conexion(t_log* logger, char* ip, char* puerto);
int crear_conexion(char *ip, char* puerto);
void pedir_seed(int conexion);
void terminar_kernel(t_log* logger, t_config* config, int conexion);

#endif /* CONEXION_H_ */
