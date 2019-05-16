/*
 * memTable.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef MEMTABLE_H_
#define MEMTABLE_H_

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
#include"APIS.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>


typedef struct nodoRegistroMemTable{
	char* value;
	uint16_t key;
	long long timestamp;
	struct nodoRegistroMemTable* siguienteRegistro;
}nodoRegistroMemTable;

typedef struct nodoTablaMemTable{
	char* nombreTabla;
	struct nodoTablaMemTable* siguiente;
	struct nodoRegistroMemTable* primerRegistro;
	struct nodoRegistroMemTable* ultimoRegistro;
}nodoTablaMemTable;





extern nodoTablaMemTable* primer; //la memtable siempre arranca vacia;
extern nodoTablaMemTable* ultimo;

void agregarTabla(nodoTablaMemTable*);
int existeDuplicado(nodoTablaMemTable* nodoTabla);
nodoTablaMemTable* crearNodoTabla(char* nombreTabla);
void imprimirListaTablas();
void eliminarNodoTabla(nodoTablaMemTable* nodoTabla);
void agregarRegistro(nodoTablaMemTable* nodoTabla, nodoRegistroMemTable* nodoRegistro);
void imprimirRegistrosTabla();
#endif /* MEMTABLE_H_ */
