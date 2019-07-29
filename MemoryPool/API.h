#ifndef API_H_
#define API_H_

#include "envio.h"
#include "conexion.h"
#include "tablas.h"

#include<pthread.h>

void selectf(int cliente_fd, t_paquete_select* paquete, t_config* config, t_log* logger);
void insert(t_paquete_insert* paquete, t_config* config, t_log* logger, int flag_modificado);
void create(int conexion, t_paquete_create* paquete, t_config* config, t_log* logger);
void journal(t_config* config, t_log* logger);
void time_journal(void * arg);

//-------------------------------JOURNAL
void enviar_modificados(char * nombre_tabla, t_list* lista ,int conexion_LFS, t_config* config, t_log* logger);
void enviar(t_pagina_completa* pagina_completa, char * nombre_tabla, int conexion_LFS,t_config* config, t_log* logger);
void eliminar_pagina_completa(void * pagina_completa);
void startup_semaforos();

#endif /* API_H_ */
