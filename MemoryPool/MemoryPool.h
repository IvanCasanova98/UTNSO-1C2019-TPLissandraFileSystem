#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<curses.h>
#include<commons/temporal.h>

#include<pthread.h>

#include "conexion.h"
#include "recibir.h"
#include "consola.h"
#include "utils.h"

int crear_hilo_servidor();
int crear_hilo_consola();

#endif /* MEMORYPOOL_H_ */
