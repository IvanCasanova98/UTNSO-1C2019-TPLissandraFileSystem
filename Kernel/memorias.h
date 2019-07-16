#ifndef MEMORIAS_H_
#define MEMORIAS_H_

#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>

#include<sys/socket.h>

#include<readline/readline.h>
#include<readline/history.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include<commons/collections/dictionary.h>
#include <sys/time.h>

#include"recibir.h"


void agregar_datos(char* nombre_tabla, char* consistencia);
bool existe_tabla(char* nombre_tabla);
void startup_diccionario();
void deserealizar_metadata(int socket);
void imprimir_diccionario(char* nombre_tabla,char* consistencia);

#endif /* MEMORIAS_H_ */
