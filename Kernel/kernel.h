#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>

#include "conexion.h"
#include "paquete.h"
#include "envio.h"

//---------------------INGRESO DE PAQUETE
void ingresar_paquete(int conexion, t_log* logger);

#endif /* KERNEL_H_ */
