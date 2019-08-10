#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>
#include<commons/collections/queue.h>

#include<readline/readline.h>
#include<readline/history.h>

#include "paquete.h"
#include "planificador.h"
#include "conexion.h"
#include "metrica.h"

void consola();
//---------------------INGRESO DE PAQUETE
int request(void * arg);
void loggearMetadataTablas(void* elemento);
void loggearListaMetadatas(t_list * metadatas);
//---------------------INGRESO DE PAQUETE(FUNC AUX)
char* ingresar_request();

#endif /* CONSOLA_H_ */
