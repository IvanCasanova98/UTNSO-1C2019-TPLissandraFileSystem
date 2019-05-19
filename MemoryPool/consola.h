#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>
#include<unistd.h>

#include<sys/socket.h>

#include<readline/readline.h>
#include<readline/history.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>

#include "envio.h"
#include "conexion.h"
#include "paquete.h"
#include "utils.h"

//---------------------INGRESO DE PAQUETE
void ingresar_paquete(void * arg);

//---------------------INGRESO DE PAQUETE(FUNC AUX)
char* ingresar_request();
int codigo_ingresado(char* lineaRequest);

#endif /* CONSOLA_H_ */
