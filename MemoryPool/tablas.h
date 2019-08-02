#ifndef TABLAS_H_
#define TABLAS_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<curses.h>
#include<commons/temporal.h>

#include<stdint.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>

#include "paquete.h"

typedef struct { //tipo de dato PAGINA
	uint16_t valor_key;
	char* value;
	long long timestamp;
} t_pagina;

typedef struct { //tipo de dato TABLA DE PAGINAS
	t_pagina* pagina;
	int flag;
} t_pagina_completa;

typedef struct {
	char* nombre_tabla;
	t_list* lista_paginas;
} t_tabla_paginas;

typedef struct{
	char* nombre_tabla;
	uint16_t particiones;
	char* consistencia;
}t_metadata;

extern t_dictionary* tabla_segmentos;

//----------------TABLA DE SEGMENTOS

void agregar_tabla( char* nombre_tabla, t_list* tabla_paginas);
bool existe_tabla_paginas(char* nombre_tabla);
t_list* buscar_tabla_paginas(char* nombre_tabla_paginas);


t_list* get_tabla_particiones();
t_list* get_nodo_metadata(char* nombre_tabla);
char * get_consistencia(char * nombre_tabla);

//---------ELIMINACION DE DATOS

void eliminar_segmento(char * nombre_tabla);
void eliminar_tabla_paginas(t_list * tabla_paginas);
void eliminar(void * pagina_completa);
void eliminar_filter(void * pagina_completa);

//----------------TABLA DE PAGINAS

bool existe_pagina(char* nombre_tabla, uint16_t valor_key);
t_list* crear_tabla_paginas(char* nombre_tabla,char* consistencia,uint16_t particiones);
void agregar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa);
t_pagina* buscar_pagina(char* nombre_pagina, uint16_t valor_key);
bool tiene_key(uint16_t key,t_pagina_completa* pagina_completa);
int cant_paginas(char* nombre_tabla);

//----------------PAGINAS

t_pagina* crear_pagina(uint16_t valor_key, char* value, long long timestamp);
t_pagina_completa* crear_pagina_completa(t_pagina* pagina);
t_pagina* devolver_pagina(t_pagina_completa* pagina_completa);

//---------------------AUXILIAR
bool condicion_select(char* nombre_tabla, uint16_t valor_key);
bool condicion_insert(t_paquete_insert * paquete, t_config * config);
bool verificar_espacio_memoria();
bool verificar_tamanio_value(uint32_t * value_long,t_config * config);
bool tabla_buscada(t_metadata* nodo, char* nombre_tabla);
uint16_t buscar_particion(char* nombre_tabla);
bool puede_reemplazar(char* nombre_tabla);
t_list* paginas_sin_modificar(char* nombre_tabla);
t_list* paginas_modificadas(t_list * lista);
bool comparar_timestamp(t_pagina_completa* pagina1, t_pagina_completa* pagina2);
void ordenar_lista(t_list* lista_paginas);
t_pagina_completa* pagina_menor_timestamp(t_list* lista_paginas);
t_pagina_completa* pagina_mayor_timestamp(t_list* lista_paginas);
void reemplazar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa,t_config * config);


void mostrar_pagina(t_pagina* pagina);
void mostrar_pagina_completa(t_pagina_completa* pagina_completa);
void mostrar_tabla_paginas(char* tabla_paginas);
void mostrar_tabla_segmentos();
void mostrar_tamanio_memoria_ocupada();
void startup_memoria();
void set_tamanio_memoria(int max_value_FS, t_config * config);
void remplazo_especifico(char* nombre_tabla, t_pagina_completa* pagina_completa, t_config* config);

#endif /* TABLAS_H_ */
