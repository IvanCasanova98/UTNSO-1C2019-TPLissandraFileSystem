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
#include<commons/collections/list.h>
#include <stdbool.h>
#include "Lissandra.h"



typedef struct nodoRegistroMemTable{
	char* value;
	uint16_t key;
	long long timestamp;

}nodoRegistroMemTable;

typedef struct nodoTablaMemTable{
	char* nombreTabla;
	t_list* registros;
}nodoTablaMemTable;

void mostrarRegistros(void* elemento);
void mostrarTablas(void* elemento);
t_list* crearMemTable();
extern nodoTablaMemTable* primer; //la memtable siempre arranca vacia;
extern nodoTablaMemTable* ultimo;
bool _mismoNombre(void* elemento);
bool igualNombre(void* elemento,char* nombreBuscado);
void agregarTabla(nodoTablaMemTable* nodoTabla,nodoRegistroMemTable* nodoRegistro);
bool existeDuplicado(nodoTablaMemTable* nodoTabla);
nodoTablaMemTable* crearNodoTabla(char* nombreTabla);
void imprimirListaTablas();
void eliminarNodoTabla();
void agregarRegistro(nodoTablaMemTable* nodoTabla, nodoRegistroMemTable* nodoRegistro);
void imprimirRegistrosTabla();
void eliminarUltimoRegistro(nodoTablaMemTable* nodoTabla);
void dump();
nodoTablaMemTable* crearNodoTabla(char* nombreTabla);
nodoRegistroMemTable* crearNodoRegistro(char*value,uint16_t key,long long timestamp);
#endif /* MEMTABLE_H_ */
