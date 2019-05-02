#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<string.h>
#include<curses.h>
#include<commons/temporal.h>
#include "conexion.h"
#include "paquete.h"
#include "envio.h"

void armar_paquete(int conexion);

#endif /* KERNEL_H_ */
