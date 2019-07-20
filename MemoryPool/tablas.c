#include "tablas.h"

//----------------TABLA DE SEGMENTOS

t_dictionary* tabla_segmentos;
t_list* tabla_particiones;

t_list* get_tabla_particiones(){return tabla_particiones;}

t_list* get_nodo_metadata(char* nombre_tabla)
{
	t_list* nodo_metadata=list_create();
	bool _tabla_buscada(void* elemento){return tabla_buscada(elemento, nombre_tabla);}
	t_metadata* nodo = list_find(tabla_particiones, _tabla_buscada);
	list_add(nodo_metadata,nodo);

	return nodo_metadata;
	list_destroy(nodo_metadata);
}

char * get_consistencia(char * nombre_tabla)
{
	t_list * lista_nodo = get_nodo_metadata(nombre_tabla);
	t_metadata * nodo = list_get(lista_nodo,0);

	return nodo->consistencia;
}

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

t_list* crear_tabla_paginas(char* nombre_tabla,char* consistencia, uint16_t particiones)
{
	t_list* tabla_paginas = list_create();

	t_metadata* metadata = malloc(strlen(nombre_tabla)+1+sizeof(uint16_t));

	metadata->nombre_tabla = malloc(strlen(nombre_tabla)+1);
	strcpy(metadata->nombre_tabla, nombre_tabla);
	metadata->particiones = particiones;
	metadata->consistencia = consistencia;
	list_add(tabla_particiones, metadata);

	agregar_tabla(nombre_tabla, tabla_paginas); //la agrega a la tabla de segmentos
	return tabla_paginas;
	free(metadata);
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

	t_pagina_completa* pagina_completa = pagina_mayor_timestamp(lista_paginas);//en este caso devuelve el primero de la lista, pero debe elegir por timestamp.

	t_pagina* pagina = pagina_completa -> pagina;

	return pagina;
}

bool tiene_key(uint16_t key,t_pagina_completa* pagina_completa)
{
	return (pagina_completa -> pagina -> valor_key) == key;
}

int cant_paginas(char* nombre_tabla)
{
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);
	return list_size(tabla_paginas);
}
//----------------PAGINAS

t_pagina* crear_pagina(uint16_t valor_key, char* value, long long timestamp)
{
	t_pagina* pagina = malloc(sizeof(t_pagina));


	pagina-> value = malloc(strlen(value)+1);
	pagina -> valor_key = valor_key;
	strcpy(pagina -> value, value);
	pagina -> timestamp = timestamp;

	return pagina;
}

t_pagina_completa* crear_pagina_completa(t_pagina* pagina)
{
	t_pagina_completa* pagina_completa = malloc(sizeof(t_pagina_completa));
	pagina_completa -> pagina = pagina;

	//CAMBIAR FLAG A 0!!
	pagina_completa -> flag = 1;
	//CAMBIAR FLAG A 0!!

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

bool condicion_insert(char* nombre_tabla)
{
	return verificar_particiones(nombre_tabla);
}

bool verificar_particiones(char* nombre_tabla)
{
	int cantidad_paginas = cant_paginas(nombre_tabla);
	uint16_t particion_max = buscar_particion(nombre_tabla);
	return particion_max>cantidad_paginas;
}

uint16_t buscar_particion(char* nombre_tabla)
{
	bool _tabla_buscada(void* elemento){return tabla_buscada(elemento, nombre_tabla);}
	t_metadata* particion = list_find(tabla_particiones, _tabla_buscada);

	return  particion->particiones;
}

bool tabla_buscada(t_metadata* nodo, char* nombre_tabla)
{
	return !strcmp(nodo->nombre_tabla, nombre_tabla);

}

bool puede_reemplazar(char* nombre_tabla)
{
	return list_size(paginas_sin_modificar(nombre_tabla))>0;
}

t_list* paginas_modificadas(t_list * lista)
{
	bool _modificadas(t_pagina_completa* elemento){return elemento->flag == 1;}

	int cantidad_paginas_modificadas= list_count_satisfying(lista,_modificadas);


	t_list* lista_modificadas = list_create();
	int i=0;
	while(i<cantidad_paginas_modificadas){
		t_pagina_completa* pagina_modificada =list_remove_by_condition(lista,_modificadas);
		list_add(lista_modificadas,pagina_modificada);
		i++;
	}

	return lista_modificadas;
}

t_list* paginas_sin_modificar(char* nombre_tabla)
{
	bool _sin_modificar(t_pagina_completa* elemento){return elemento->flag == 0;}

	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);
	t_list* lista_pagina_sin_modificar = list_filter(tabla_paginas, _sin_modificar);

	return lista_pagina_sin_modificar;
}


bool comparar_timestamp(t_pagina_completa* pagina1, t_pagina_completa* pagina2)
{
	return (pagina1->pagina->timestamp)<(pagina2->pagina->timestamp);
}

void ordenar_lista(t_list* lista_paginas)//Ordena lista de paginas completas, por timestamp, de menor a mayor
{
	list_sort(lista_paginas, comparar_timestamp);
}

t_pagina_completa* pagina_menor_timestamp(t_list* lista_paginas)
{
	ordenar_lista(lista_paginas);
	return list_get(lista_paginas, 0);
}
t_pagina_completa* pagina_mayor_timestamp(t_list* lista_paginas)
{
	ordenar_lista(lista_paginas);
	int ultimo = list_size(lista_paginas);
	ultimo --;
	return list_get(lista_paginas, ultimo);
}


void reemplazar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa)
{
	t_list* lista_paginas_sin_modificar = paginas_sin_modificar(nombre_tabla);
	t_pagina_completa* pagina_con_menor_timestamp = pagina_menor_timestamp(lista_paginas_sin_modificar);

	t_list* tabla_paginas= buscar_tabla_paginas(nombre_tabla);


	int i = 0;
	t_pagina_completa* pagina_A_Reemplazar = list_get(tabla_paginas,i);
	while(pagina_con_menor_timestamp->pagina->timestamp != pagina_A_Reemplazar->pagina->timestamp){
		i++;
		pagina_A_Reemplazar = list_get(tabla_paginas,i);
	}
	list_replace(tabla_paginas,i,pagina_completa);
}
//-----------------------MOSTRAR ELEMENTOS

void mostrar_pagina(t_pagina* pagina)
{
	printf("%d   %s  %lld", pagina -> valor_key, pagina -> value, pagina -> timestamp);
}

void mostrar_pagina_completa(t_pagina_completa* pagina_completa)
{
	printf("%d   %s  %lld %d. \n", pagina_completa -> pagina -> valor_key, pagina_completa -> pagina -> value, pagina_completa -> pagina -> timestamp, pagina_completa -> flag);
}


void mostrar_tabla_paginas(char* tabla_paginas)//mostrar todas las paginas de la tabla
{
//	printf("%s", tabla_paginas);
	list_iterate(buscar_tabla_paginas(tabla_paginas), mostrar_pagina_completa);
}

void mostrar_tabla_segmentos()
{
	dictionary_iterator(tabla_segmentos,mostrar_tabla_paginas);
	printf("\n");
}


void startup_memoria()
{
	tabla_segmentos = dictionary_create();
	tabla_particiones = list_create();


	t_list* tabla_paginas0 = crear_tabla_paginas("TABLA0","SC",4);
	t_list* tabla_paginas1 = crear_tabla_paginas("TABLA1","SC",4);
	t_list* tabla_paginas2 = crear_tabla_paginas("TABLA2","SC",7);

	t_pagina* pagina0 = crear_pagina(0, "CERO", 12);
	t_pagina* pagina1 = crear_pagina(1, "UNO", 34);
	t_pagina* pagina2 = crear_pagina(2, "DOS", 56);
	t_pagina* pagina3 = crear_pagina(3, "TRES", 178);
	t_pagina* pagina4 = crear_pagina(4, "CUATRO", 90);

	t_pagina_completa* pagina_completa0 = crear_pagina_completa(pagina0);
	t_pagina_completa* pagina_completa1 = crear_pagina_completa(pagina1);
	t_pagina_completa* pagina_completa2 = crear_pagina_completa(pagina2);
	t_pagina_completa* pagina_completa3 = crear_pagina_completa(pagina3);
	t_pagina_completa* pagina_completa4 = crear_pagina_completa(pagina4);

	agregar_pagina("TABLA0", pagina_completa0);
	agregar_pagina("TABLA0", pagina_completa3);
	agregar_pagina("TABLA0", pagina_completa4);
	agregar_pagina("TABLA1", pagina_completa1);
	agregar_pagina("TABLA2", pagina_completa2);

//	mostrar_tabla_segmentos();

}
