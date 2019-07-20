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

void* serializar_mensaje(char* value, int bytes);
void* serializar_array_int(int array[], int bytes, int cant_elementos);
void* serializar_array(char** array, int bytes, int cant_elementos);
void* serializar_enviar_paquete_describe(int socket_cliente, t_list* metadata);
void* serealizar_seed_completa(int memoria, int puerto, char* IP,int tamanio_total);


void* serializar_paquete_select(t_paquete_select* paquete);
void* serializar_paquete_insert(t_paquete_insert* paquete);
void* serializar_paquete_create(t_paquete_create* paquete);
void* serializar_paquete_drop(t_paquete_drop* paquete);
void* serializar_paquete_describe(t_paquete_describe_lfs* paquete);
//---------------------------ENVIOS DE MEMORIAS

void enviar_memorias(int socket_cliente, t_config* config);

//----------------------------ENVIAR PAQUETES

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente, t_log* logger);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente, t_log* logger);
void enviar_paquete_create(t_paquete_create* paquete, int socket_cliente, t_log* logger);
void enviar_paquete_drop(t_paquete_drop* paquete, int socket_cliente,t_log* logger);

//----------------------------ENVIOS DE SERVIDOR A LISSANDRA

void* enviar_select_lissandra(t_paquete_select* paquete, t_config* config, t_log* logger);
void enviar_insert_lissandra(t_paquete_insert* paquete, t_config* config, t_log* logger);
void enviar_describe_lissandra(t_paquete_describe_lfs* paquete_describe_lfs,t_config* config,t_log* logger);
void enviar_create_lissandra(t_paquete_create* paquete_c,t_config* config,t_log* logger);
void enviar_drop_lissandra(t_paquete_create* paquete,t_config* config,t_log* logger);

//----------------------------EXTRAS LFS
void loggearListaMetadatas(t_dictionary * metadatas);
void loggearMetadataTablas(char*nombreTabla,void* elemento);

#endif /* ENVIO_H_ */
