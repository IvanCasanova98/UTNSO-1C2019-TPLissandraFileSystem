#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<stdbool.h>
#include<commons/collections/queue.h>

#include "utils.h"

int contador_hilos;

typedef struct
{
	int boolean;
	void * elemento;
}t_proceso;

int tamanio(char ** vector);

//-------------------------PLANIFICADOR--------------------------
void planificador(void * arg);
void dispatcher();
//------------------------------HILOS----------------------------
void crear_hilo(void* arg);
//-------------------------COLA DE READY-------------------------
//CREAR COLA
void startup_cola_ready();
//AGREGAR A COLA
void agregar_a_cola(char* request);
//EXTRAER DE LA COLA
void* pop_elemento();
//HAY ELEMENTOS EN LA COLA
bool elementos_en_la_cola();

#endif /* PLANIFICADOR_H_ */
