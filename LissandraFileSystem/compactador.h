/*
 * compactador.h
 *
 *  Created on: 13 jun. 2019
 *      Author: utnso
 */

#ifndef COMPACTADOR_H_
#define COMPACTADOR_H_


#include <dirent.h>
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
#include <sys/inotify.h>
#include <unistd.h>
#include"recibir.h"
#include"memTable.h"
#include "bloque.h"
#include "Lissandra.h"
#include "bitmap.h"
#include"logs.h"
void* concatenar(char*registrosCompletos,char* registrosDeBloque);
void* crearParticionNueva(char* nombreTabla,char* registros,int nroParticion);
void cargarRegistros(char* registroActual, int* bloquesDisponibles);
void crearArchivobin(char* nombreTabla, int size, int*bloques,int cantBloques,int nroParticion);
char* crearRegistroACargar(t_list* ListaRegistros);
void* transformarARegistroChar(void* registro);
char* serializarRegistroBarraN(char* value,uint16_t key,long long timestamp);
void* concatenar(char*registrosCompletos,char* registrosDeBloque);
t_list* ActualizarRegistrosParticion(t_list* ListaRegistrosParticion,t_list* ListaRegistrosTemporalCompactados);
void* CompararRegistros(void*registroTemporal,t_list* ListaRegistrosParticion);
bool CambiarRegistro(void*registroParticion,void*registroTemporal);
bool EstaLaKey(void* registro ,t_registro* registroTemporalCasteado);
bool filtrarPorMismaKey(void* registro,int nroParticion,int NumeroParticiones);
void RemoverTemporalesCompactadosDeTablaYSusBloques(char* nombretabla);
void cambiarNombreTmpATmpc(char* nombretabla);
void* compactar(char* nombreTabla);
void* imprimirRegistro(void*registro);
char* serializarRegistroBarraN(char* value,uint16_t key,long long timestamp);
t_list* LiberarTmpc(char *nombreTabla);
t_list* LiberarBin(char *nombreTabla,int NroBin);
#endif /* COMPACTADOR_H_ */
