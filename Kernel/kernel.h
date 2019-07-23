#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#include "conexion.h"
//#include "paquete.h"
#include "envio.h"
#include "recibir.h"
#include "criterio.h"
#include "utils.h"
#include "memorias.h"
#include "consola.h"
#include "planificador.h"


void levantar_kernel(t_config* config, t_log* logger);

#endif /* KERNEL_H_ */
