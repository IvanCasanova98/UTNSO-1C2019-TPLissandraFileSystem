#ifndef KERNELCLIENTE_H_
#define KERNELCLIENTE_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"

t_log* iniciar_logger(void);
t_config* leer_config(void);
void leer_consola(t_log* logger);
t_paquete* armar_paquete(op_code, char, int); //ParametroAgregado

void _leer_consola_haciendo(void(*accion)(char*));

void iniciarCliente(void); //Agregado

t_paquete* construirDatos(); //Agregado
void mostrarPaquete(t_paquete*); //Agregado


#endif /* KERNELCLIENTE_H_ */
