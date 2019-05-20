/*
 * bitmap.h
 *
 *  Created on: 18 may. 2019
 *      Author: utnso
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>
#include<sys/socket.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<commons/log.h>
#include<commons/error.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include"recibir.h"
#include"memTable.h"
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<commons/bitarray.h>
#include <stdbool.h>
#include<commons/collections/dictionary.h>
#include "Lissandra.h"
#include <commons/collections/node.h>
#include "commons/txt.h"
#include <sys/mman.h>


char* eliminarBarra0(char* nuevoChar);
int existeBitmap();
void crearBitMap();
void ActualizarBitmap();
void pruebasSet();
void crearArchivoBitmap();
#endif /* BITMAP_H_ */
