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




argumentosEnvioPaquete* crearArgumentosDeConexion(int clientefd, int serverfd);


#endif /* LISSANDRA_H_ */
