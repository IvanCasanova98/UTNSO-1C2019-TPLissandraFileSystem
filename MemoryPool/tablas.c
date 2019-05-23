#include "tablas.h"

//----------------TABLA DE SEGMENTOS
t_dictionary* tabla_segmentos;

void agregar_tabla( char* nombre_tabla, t_list* tabla_paginas)//no debe recibir tabla_segmentos (variable global)
{
	dictionary_put(tabla_segmentos, nombre_tabla, tabla_paginas);
}

bool existe_tabla_paginas(char* nombre_tabla)
{
	return dictionary_has_key(tabla_segmentos, nombre_tabla);
}

t_list* buscar_tabla_paginas(char* nombre_tabla_paginas){
	return dictionary_get(tabla_segmentos, nombre_tabla_paginas);
}

//----------------TABLA DE PAGINAS

t_list* crear_tabla_paginas(char* nombre_tabla)
{
	//fijarse si ya esta creada, sino...
	t_list* tabla_paginas = list_create();
	agregar_tabla(nombre_tabla, tabla_paginas); //la agrega a la tabla de segmentos
	return tabla_paginas;
}

void agregar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa) //a tabla de paginas
{
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);
	list_add(tabla_paginas, pagina_completa);
}

bool existe_pagina(char* nombre_tabla, uint16_t valor_key)
{
	bool _tiene_key(void* elemento){return tiene_key(valor_key, elemento);}
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);

	return list_any_satisfy(tabla_paginas, _tiene_key);
}

t_pagina* buscar_pagina(char* nombre_tabla, uint16_t valor_key)
{
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);

	bool _tiene_key(void* elemento){return tiene_key(valor_key, elemento);}//esto es posiblie gracias a que usamos GCC, no es parte del estandar de C. Funcion interna (funcion definidia adentro de otra)

	t_list* lista_paginas = list_filter(tabla_paginas, _tiene_key);//devuelve una nueva lista de paginas que tienen esta key. (falta elegir por timestamp)

	t_pagina_completa* pagina_completa = list_get(lista_paginas,0);//en este caso devuelve el primero de la lista, pero debe elegir por timestamp.

	t_pagina* pagina = pagina_completa -> pagina;

	return pagina;
}

bool tiene_key(uint16_t key,t_pagina_completa* pagina_completa)
{
	return (pagina_completa -> pagina -> valor_key) == key;
}
//----------------PAGINAS

t_pagina* crear_pagina(uint16_t valor_key, char* value, long long timestamp)
{
	t_pagina* pagina = malloc(sizeof(t_pagina));

	pagina -> valor_key = valor_key;
	pagina -> value = value;
	pagina -> timestamp = timestamp;

	return pagina;
}

t_pagina_completa* crear_pagina_completa(t_pagina* pagina)
{
	t_pagina_completa* pagina_completa = malloc(sizeof(t_pagina_completa));
	pagina_completa -> pagina = pagina;
	pagina_completa -> flag = 0;

	return pagina_completa;
}

t_pagina* devolver_pagina(t_pagina_completa* pagina_completa)
{
	return pagina_completa->pagina;
}

//----------------------------------------------------AUXILIARES-----------------------------------------------------------

bool condicion_select(char* nombre_tabla, uint16_t valor_key) //Verifica la existencia de la tabla y luego de la pagina que contenga la KEY
{
	return existe_tabla_paginas(nombre_tabla) && existe_pagina(nombre_tabla, valor_key);
}

//-----------------------MOSTRAR ELEMENTOS

void mostrar_pagina(t_pagina* pagina)
{
	printf("%d   %s  %lld", pagina -> valor_key, pagina -> value, pagina -> timestamp);
}

void mostrar_pagina_completa(t_pagina_completa* pagina_completa)
{
	printf("%d   %s  %lld %d", pagina_completa -> pagina -> valor_key, pagina_completa -> pagina -> value, pagina_completa -> pagina -> timestamp, pagina_completa -> flag);
}

void mostrar_tabla_paginas(t_tabla_paginas* tabla_paginas)//mostrar todas las paginas de la tabla
{
	list_iterate((t_list*)tabla_paginas, mostrar_pagina_completa);
}

void mostrar_tabla_segmentos()
{
	dictionary_iterator(tabla_segmentos,mostrar_tabla_paginas);
	printf("\n");
}

//

void startup_memoria()
{
	tabla_segmentos = dictionary_create();

	t_list* tabla_paginas0 = crear_tabla_paginas("TABLA0");
	t_list* tabla_paginas1 = crear_tabla_paginas("TABLA1");
	t_list* tabla_paginas2 = crear_tabla_paginas("TABLA2");

	t_pagina* pagina0 = crear_pagina(0, "cero", 12);
	t_pagina* pagina1 = crear_pagina(1, "uno", 34);
	t_pagina* pagina2 = crear_pagina(2, "dos", 56);

	t_pagina_completa* pagina_completa0 = crear_pagina_completa(pagina0);
	t_pagina_completa* pagina_completa1 = crear_pagina_completa(pagina1);
	t_pagina_completa* pagina_completa2 = crear_pagina_completa(pagina2);

	agregar_pagina("TABLA0", pagina_completa0);
	agregar_pagina("TABLA1", pagina_completa1);
	agregar_pagina("TABLA2", pagina_completa2);

}
