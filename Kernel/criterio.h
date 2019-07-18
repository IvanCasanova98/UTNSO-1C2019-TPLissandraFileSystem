#ifndef CRITERIO_H_
#define CRITERIO_H_

#include "utils.h"
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include<commons/collections/list.h>

#include "memorias.h"

extern t_list* lista_seeds;

void startup_lista_seeds();


void mostrar_lista_seeds();
void imprimir(SEED * seed);
SEED * elegir_memoria(char * consistencia_tabla);

SEED * get_seed_especifica(int numero_memoria);
bool seed_buscada(SEED * memoria_i , int numero_memoria);

//SEED elegir_memoria();

#endif /* CRITERIO_H_ */
