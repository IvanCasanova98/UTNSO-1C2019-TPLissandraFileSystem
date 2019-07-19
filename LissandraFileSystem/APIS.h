/*
 * APIS.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef APIS_H_
#define APIS_H_



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
t_config config;

#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"
#define NORMAL_COLOR  "\x1B[0m"

int existeTabla(char* TablaBuscada);

//**********- FUNCIONES API -**********
void  APIdrop(t_paquete_drop*);
char* APIselect(t_paquete_select*);
void  APIinsert(t_paquete_insert*);
void  APIcreate(t_paquete_create*);
t_dictionary*  APIdescribe(t_paquete_describe*);
t_dictionary*  APIdescribeTodasLasTablas();

//-------------------------------RESPUESTAS

respuestaSELECT* APIselectRESPUESTA(t_paquete_select* paquete_select);
uint16_t APIinsertRESPUESTA(t_paquete_insert* paquete_insert);
uint16_t APIcreateRESPUESTA(t_paquete_create* paquete_create);
uint16_t APIdropRESPUESTA(t_paquete_drop* paquete_drop);
void* APIdescribeTodasLasTablasRESPUESTA();
respuestaDESCRIBE* APIdescribeRESPUESTA(t_paquete_describe* paquete_describe);

//*************************************
void liberarRegistro(void* registro);
void verificarSemaforoMemTable();
void verificarSemaforoCompactacion(char* nombreTabla);
void verificarSemaforoTabla(char* nombreTabla);
bool validarConsistencia(char* consistencia);
void liberarRegistro(void* elemento);
void liberarPaqueteCreate(t_paquete_create* paquete_create);
bool mayorTimeStampRegistro(void*elemento1,void*elemento2);
t_registro* buscarEnTemporales(char* nombreTabla,int key);
bool filtrarPorKeyRegistro(void* elemento,int key);
t_registro* buscarRegistroTemporalMasReciente(char* todosLosRegistrosTemporales,int key);
char* DirectorioDeTemporal(char* nombretabla,int nroTemporal);
t_registro* buscarKey(char* registrosDeBloque,int key);
t_registro* buscarEnParticion(char* nombreTabla, int nroParticion,int key);
char* DirectorioDeTemporalNuevo(char* nombretabla);

void mostrarMetadataTablas(char*nombreTabla,void* elemento);
void imprimirListaMetadatas(t_dictionary * metadatas);
void imprimirMetadataDeTabla(char* nombre_tabla);
void imprimirMetadata(t_metadata_fs* metadataDeTablaPedida);
void listarTablas(); //ESTA FUNCION NO SIRVE PARA NADA, SOLO POR SI QUEREMOS LISTARLAS.
t_list* listarTablasExistentes();
char* DirectorioDeTemporalCompactacion(char* nombretabla,int nroTemporal);
char* DirectorioDeTemporalNuevo(char* nombretabla);
t_registro* buscarEnTemporalesCompactando(char* nombreTabla,int key);
int existe_temporal(char* path);
char* DirectorioDeBloque(int numeroBloque);
void crearParticiones(char*nombreTabla ,int nroParticiones);
char* elegirMayorTimeStamp(t_list* RegistrosEncontrados);
t_registro* crearRegistro (char*value,uint16_t key,long long timestamp);
int particionDeKey(int key,int particiones);
t_metadata_fs* obtenerMetadataTabla (char* nombreTabla );
char* DirectorioDeTabla(char* tabla);
t_metadata_fs* crearMetadata(consistency consistencia, int particiones,int tiempo_compactacion);
int pasarAConsistenciaINT(char* consistencia);
char* pasarAConsistenciaChar(int consistencia);
void logLaTablaYaExiste();
void crearTabla(char* tabla);
char* pasarMayuscula(char*);
char* DirectorioDeMetadataTabla(char* tabla);
void crearMetadataConfig(char*nombreTablaMayuscula, consistency consistencia, int particiones,int tiempo_compactacion);
char* DirectorioDeMetadata();
char* DirectorioBitMap();
void crearParticiones(char*nombreTabla ,int particiones);
char* DirectorioDeParticion(char* ,int numeroParticion);
void crearParticion(char*nombreTabla ,int particiones);

void removerBloque(char* nroBloque);
void RemoverTemporalesDeTablaYSusBloques(char* nombretabla);
void RemoverMetadataDeTabla(char *nombreTabla);
void RemoverCarpetaVaciaDeTabla(char *nombreTabla);
void RemoverDeLaMemtable(char *nombreTabla);
void RemoverParticionesDeTablaYSusBloques(char* nombreTabla);
void RemoverParticionDeTablaEspecificaYSusBloques(char* nombreTabla,int nroParticionABorrar);

#endif /* APIS_H_ */
