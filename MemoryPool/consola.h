#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

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

//---------------------ARCHIVO LOGGER/CONFIG
t_log* iniciar_logger(void);
t_config* leer_config(void);

//----------------------------ENVIAR PAQUETES
void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente);

//---------------------INGRESO DE PAQUETE
void ingresar_paquete();

//---------------------INGRESO DE PAQUETE(FUNC AUX)
char* ingresar_request();
int codigo_ingresado(char* lineaRequest);

#endif /* CONSOLA_H_ */
