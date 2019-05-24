/*
 * logs.h
 *
 *  Created on: 20 may. 2019
 *      Author: utnso
 */

#ifndef LOGS_H_
#define LOGS_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include <stdbool.h>
#include<curses.h>
#include<sys/socket.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<commons/log.h>
#include<commons/error.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include<commons/bitarray.h>
#include<commons/collections/dictionary.h>
#include <commons/collections/node.h>
#include <commons/txt.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include"recibir.h"
#include"memTable.h"
#include "Lissandra.h"
#include"logs.h"

void LogearInsert(long long timestamp,uint16_t key,char* value,char*nombreTabla);
void LogearCreate(char*nombreTabla);
void LogearSelect(long long timestamp,uint16_t key,char* value,char*nombreTabla);
void LaTablaNoExiste(long long timestamp,uint16_t key,char* value,char*nombreTabla);
void LaTablaYaExiste(long long timestamp,uint16_t key,char* value,char*nombreTabla);
void LogDumpeo();


#endif /* LOGS_H_ */
