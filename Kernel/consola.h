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

void consola();
//---------------------INGRESO DE PAQUETE
void request(void * arg);

//---------------------INGRESO DE PAQUETE(FUNC AUX)
char* ingresar_request();

#endif /* CONSOLA_H_ */
