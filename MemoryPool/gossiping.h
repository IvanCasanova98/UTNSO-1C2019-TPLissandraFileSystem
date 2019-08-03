#ifndef GOSSIPING_H_
#define GOSSIPING_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<curses.h>
#include<commons/temporal.h>

#include<stdint.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include "conexion.h"



extern t_list * tabla_gossip;

//--------------------------GOSSIPING
void startup_tabla_gossip(t_config * config);
t_list * get_tabla_gossip();

void gossiping(void * arg);

void mostrar_tabla_gossip();
void imprimir_memoria(SEED * seed);

bool table_has_memory(int numero_memoria);


#endif /* GOSSIPING_H_ */
