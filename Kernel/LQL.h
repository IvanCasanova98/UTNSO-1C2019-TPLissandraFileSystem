#ifndef LQL_H_
#define LQL_H_

#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#include "paquete.h"

void funcion_LQL(char* path, int conexion);
void seleccionar_paquete(char* lineaRequest, int conexion);

#endif /* LQL_H_ */
