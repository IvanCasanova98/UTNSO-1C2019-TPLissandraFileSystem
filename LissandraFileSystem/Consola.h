/*
 * Consola.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <ctype.h>
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
#include <sys/time.h>
#include"recibir.h"
#include"memTable.h"
#include"bloque.h"
#include"compactador.h"
#include"Lissandra.h"
#include"bitmap.h"
#include"APIS.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
void deployMenu();
//---------------------INGRESO DE PAQUETE
char* ingresar_request();
void* prenderConsola(void* );
int codigo_ingresado(char* codOp);

//---------------------CREAR PAQUETE
t_paquete_select* 	crear_paquete_select(char* nombretabla,uint16_t valor_key);
t_paquete_insert* 	crear_paquete_insert(char* nombretabla,uint16_t valor_key, char *value, long long timestamp);
t_paquete_create* 	crear_paquete_create(char* nombretabla, consistency consistencia, int particiones,int tiempo_de_compactacion);
t_paquete_drop*   	crear_paquete_drop(char *nombretabla);
t_paquete_describe* crear_paquete_describe(char *nombre_tabla);
bool validarNumero(char* parametro);

//---------------------ARMAR PAQUETE
t_paquete_select* 	LeerSelect(char* parametros);
t_paquete_insert* 	LeerInsert(char* parametros);
t_paquete_create* 	LeerCreate(char* parametros);
t_paquete_drop*   	LeerDrop(char *parametros);
t_paquete_describe* LeerDescribe(char *parametros);
t_paquete_insert* LeerInsert2(char* parametros);
//---------------------LOGGEAR PAQUETE
void loggear_request_select(t_paquete_select* paquete);
void loggear_request_insert(t_paquete_insert* paquete);
void loggear_request_create(t_paquete_create* paquete);
void loggear_request_drop(t_paquete_drop* paquete);
void loggear_request_describe_con_tabla(t_paquete_describe* paquete);
void loggear_request_describe_sin_tabla();


#endif /* CONSOLA_H_ */
