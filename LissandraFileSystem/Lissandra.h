#ifndef LISSANDRA_H_
#define LISSANDRA_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<curses.h>
#include<commons/temporal.h>
#include<pthread.h>
#include "conexion.h"
#include "recibir.h"
#include "Consola.h"
#include "APIS.h"
#include "memTable.h"
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include"bitmap.h"
#include<time.h>
#include <semaphore.h>


extern sem_t SemaforoMemtable;
extern sem_t SemaforobitArray;
extern t_list* ListaSem;
extern t_dictionary* TablasSem;
extern t_dictionary* TablasCompactacion;
extern t_dictionary* memTable;
extern t_bitarray* bitmap;
extern struct timeval tiempoHastaDump;

//extern struct timespec stopDump;
extern bool estaDump;
#endif /* LISSANDRA_H_ */
