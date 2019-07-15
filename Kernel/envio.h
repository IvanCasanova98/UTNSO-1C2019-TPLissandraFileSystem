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
#include "utils.h"


//----------------------------SERIALIZAR PAQUETES
void* serializar_paquete_select(t_paquete_select* paquete);
void* serializar_paquete_insert(t_paquete_insert* paquete);
void* serializar_paquete_create(t_paquete_create* paquete);
void* serializar_paquete_drop(t_paquete_drop* paquete);

//----------------------------ENVIAR PAQUETES
void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente);
void enviar_paquete_create(t_paquete_create* paquete, int socket_cliente);


#endif /* ENVIO_H_ */
