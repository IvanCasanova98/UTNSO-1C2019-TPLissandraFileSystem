/*
 * APIS.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef APIS_H_
#define APIS_H_
#define DIRECTORIOTABLAS "/home/utnso/lfs/LissandraFileSystem/Tablas/"

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
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>


int existeTabla(char* TablaBuscada);

char* APIselect(t_paquete_select*);

void APIinsert(t_paquete_insert*);


#endif /* APIS_H_ */
