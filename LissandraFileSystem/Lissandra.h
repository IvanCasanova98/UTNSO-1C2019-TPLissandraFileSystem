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


extern t_dictionary* memTable;
extern t_bitarray* bitmap;
#endif /* LISSANDRA_H_ */
