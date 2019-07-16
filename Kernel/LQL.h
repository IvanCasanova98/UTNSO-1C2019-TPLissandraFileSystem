#ifndef LQL_H_
#define LQL_H_

#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<commons/collections/queue.h>

#include "paquete.h"

t_queue* lql_to_queue(char* path);
int tamanio_lql(t_queue* proceso_lql);
//void funcion_LQL(char* path, int conexion, t_config* config);

#endif /* LQL_H_ */
