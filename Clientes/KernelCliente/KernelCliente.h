#ifndef KERNELCLIENTE_H_
#define KERNELCLIENTE_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <string.h>


#include "utils.h"

t_log* iniciar_logger(void);
t_config* leer_config(void);
void leer_consola(t_log* logger);
t_paquete* armar_paquete(op_code cod_op,char *nombretabla, int valor_key);

void _leer_consola_haciendo(void(*accion)(char*));

void iniciarCliente(void); //Agregado
t_paquete* construir_paquete();


void mostrarPaquete(t_paquete*);
void loggear_paquete(t_paquete* paquete, char tipo_paquete[10]);
char enteroACaracter(int numero);


#endif /* KERNELCLIENTE_H_ */
