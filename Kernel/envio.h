#ifndef ENVIO_H_
#define ENVIO_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

#include "paquete.h"

t_log* iniciar_logger(void);
t_config* leer_config(void);

void enviar_paqueteSELECT(t_paqueteSELECT* paquete, int socket_cliente);
void enviar_paqueteINSERT(t_paqueteINSERT* paquete, int socket_cliente);

#endif /* ENVIO_H_ */
