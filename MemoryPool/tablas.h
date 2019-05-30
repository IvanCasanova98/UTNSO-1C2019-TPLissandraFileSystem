#ifndef TABLAS_H_
#define TABLAS_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/log.h>
#include<curses.h>
#include<commons/temporal.h>

#include<stdint.h>
#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>

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

//----------------TABLA DE SEGMENTOS

void agregar_tabla( char* nombre_tabla, t_list* tabla_paginas);
bool existe_tabla_paginas(char* nombre_tabla);
t_list* buscar_tabla_paginas(char* nombre_tabla_paginas);

//----------------TABLA DE PAGINAS

bool existe_pagina(char* nombre_tabla, uint16_t valor_key);
t_list* crear_tabla_paginas(char* nombre_tabla);
void agregar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa);
t_pagina* buscar_pagina(char* nombre_pagina, uint16_t valor_key);
bool tiene_key(uint16_t key,t_pagina_completa* pagina_completa);

//----------------PAGINAS

t_pagina* crear_pagina(uint16_t valor_key, char* value, long long timestamp);
t_pagina_completa* crear_pagina_completa(t_pagina* pagina);
t_pagina* devolver_pagina(t_pagina_completa* pagina_completa);

//---------------------AUXILIAR
bool condicion_select(char* nombre_tabla, uint16_t valor_key);
void mostrar_pagina(t_pagina* pagina);
void mostrar_pagina_completa(t_pagina_completa* pagina_completa);
void mostrar_tabla_paginas(t_tabla_paginas* tabla_paginas);
void mostrar_tabla_segmentos();
void startup_memoria();

#endif /* TABLAS_H_ */