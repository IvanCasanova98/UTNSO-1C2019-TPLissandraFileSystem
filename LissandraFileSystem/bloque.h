/*
 * bloque.h
 *
 *  Created on: 21 may. 2019
 *      Author: utnso
 */

#ifndef BLOQUE_H_
#define BLOQUE_H_
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
#include "bitmap.h"
#include"logs.h"


char* serializarRegistro(char* value,uint16_t key,long long timestamp);

#endif /* BLOQUE_H_ */
