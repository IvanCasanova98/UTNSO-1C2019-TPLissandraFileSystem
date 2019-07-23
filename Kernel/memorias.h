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
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include <sys/time.h>

#include"recibir.h"

void startup_diccionario();

void agregar_datos(char* nombre_tabla, char* consistencia);
void agregar_consistencia(int memoria, char* consistencia);
t_list * get_pool(int cons_ingresada);
char * get_consistencia(char * nombre_tabla);

bool existe_tabla(char* nombre_tabla);
bool verificar_consistencias();


void deserealizar_metadata(int socket);
void imprimir_consistencias();
void imprimir_diccionario(char* nombre_tabla,char* consistencia);

void journal_memorias(t_log* logger);

#endif /* MEMORIAS_H_ */
