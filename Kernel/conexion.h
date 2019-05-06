#ifndef CONEXION_H_
#define CONEXION_H_

#include<netdb.h>
#include<string.h>
#include<unistd.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

int iniciar_conexion(t_log* logger, t_config* config);
int crear_conexion(char *ip, char* puerto);
void terminar_conexion(t_log* logger, t_config* config, int conexion);

#endif /* CONEXION_H_ */
