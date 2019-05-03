/*
 * envio.h
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#ifndef SERVIDORES_MEMORYPOOLSERVIDOR_ENVIO_H_
#define SERVIDORES_MEMORYPOOLSERVIDOR_ENVIO_H_

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



#endif /* SERVIDORES_MEMORYPOOLSERVIDOR_ENVIO_H_ */
