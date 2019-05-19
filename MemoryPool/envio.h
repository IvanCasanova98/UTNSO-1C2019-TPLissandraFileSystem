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
#include "conexion.h"
#include "utils.h"

//----------------------------SERIALIZAR PAQUETES
void* serializar_paquete_select(t_paquete_select* paquete);
void* serializar_paquete_insert(t_paquete_insert* paquete);

//----------------------------ENVIAR PAQUETES
void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente, t_log* logger);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente, t_log* logger);

//----------------------------ENVIOS DE SERVIDOR A LISSANDRA
void enviar_select_lissandra(t_paquete_select* paquete, t_config* config, t_log* logger);
void enviar_insert_lissandra(t_paquete_insert* paquete, t_config* config, t_log* logger);

#endif /* ENVIO_H_ */
