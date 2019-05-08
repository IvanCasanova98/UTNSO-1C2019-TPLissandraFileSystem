/*
 * Consola.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include<stdio.h>
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
#include"recibir.h"
//---------------------INGRESO DE PAQUETE
char* ingresar_request();
void* prenderConsola(void* );
int codigo_ingresado(char* codOp);


//---------------------CREAR PAQUETE
t_paquete_select* crear_paquete_select(char *nombretabla,uint16_t valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,uint16_t valor_key, char *value, int timestamp);

//---------------------ARMAR PAQUETE
t_paquete_select* selectf(char* parametros);
t_paquete_insert* insert(char* parametros);

//---------------------LOGGEAR PAQUETE
void loggear_request_select(t_paquete_select* paquete);
void loggear_request_insert(t_paquete_insert* paquete);
#endif /* CONSOLA_H_ */
