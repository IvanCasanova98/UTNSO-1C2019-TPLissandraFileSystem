#include "tablas.h"

//----------------TABLA DE SEGMENTOS

int max_paginas_disponibles;

int tamanio_memoria_ocupada;
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

//---------ELIMINACION DE DATOS
void eliminar_segmento(char * nombre_tabla)
{
	void _eliminar_tabla_paginas(void * tabla_paginas){eliminar_tabla_paginas(tabla_paginas);}

	dictionary_remove_and_destroy(tabla_segmentos,nombre_tabla,_eliminar_tabla_paginas);
}

void eliminar_tabla_paginas(t_list * tabla_paginas)
{
	list_destroy_and_destroy_elements(tabla_paginas,eliminar);
}

void eliminar(void * pagina_completa)
{

	tamanio_memoria_ocupada -= 1;

	free(((t_pagina_completa *) pagina_completa)->pagina->value);
	free(((t_pagina_completa *) pagina_completa)->pagina);
	free(((t_pagina_completa *) pagina_completa));
}

void eliminar_filter(void * pagina_completa)
{
	free(((t_pagina_completa *) pagina_completa)->pagina->value);
	free(((t_pagina_completa *) pagina_completa)->pagina);
	free(((t_pagina_completa *) pagina_completa));
}

//----------------TABLA DE PAGINAS

t_list* crear_tabla_paginas(char* nombre_tabla,char* consistencia, uint16_t particiones)
{
	t_list* tabla_paginas = list_create();

	agregar_tabla(nombre_tabla, tabla_paginas); //la agrega a la tabla de segmentos

	return tabla_paginas;
}

void agregar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa) //a tabla de paginas
{
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);
	list_add(tabla_paginas, pagina_completa);

	tamanio_memoria_ocupada += 1 ;
}

bool existe_pagina(char* nombre_tabla, uint16_t valor_key)
{
	bool _tiene_key(void* elemento){return tiene_key(valor_key, elemento);}
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);

	if(existe_tabla_paginas(nombre_tabla))
	{
		return list_any_satisfy(tabla_paginas, _tiene_key);
	}
	else
	{
		return false;
	}

}


void remplazo_especifico(char* nombre_tabla, t_pagina_completa* pagina_completa, t_config* config)
{
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);

	bool _misma_key(void* elemento){return tiene_key(pagina_completa->pagina->valor_key, elemento);}

	list_remove_and_destroy_by_condition(tabla_paginas, _misma_key, eliminar);

	agregar_pagina(nombre_tabla, pagina_completa);
}

t_pagina* buscar_pagina(char* nombre_tabla, uint16_t valor_key)
{
	t_list* tabla_paginas = buscar_tabla_paginas(nombre_tabla);

	bool _tiene_key(void* elemento){return tiene_key(valor_key, elemento);}//esto es posiblie gracias a que usamos GCC, no es parte del estandar de C. Funcion interna (funcion definidia adentro de otra)

	t_list* lista_paginas = list_filter(tabla_paginas, _tiene_key);//devuelve una nueva lista de paginas que tienen esta key. (falta elegir por timestamp)

	t_pagina_completa* pagina_completa = pagina_mayor_timestamp(lista_paginas);//en este caso devuelve el primero de la lista, pero debe elegir por timestamp.

	t_pagina* pagina = pagina_completa -> pagina;

	list_destroy(lista_paginas);

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
//	return existe_tabla_paginas(nombre_tabla) && existe_pagina(nombre_tabla, valor_key);
	return existe_pagina(nombre_tabla, valor_key);
}

bool condicion_insert(t_paquete_insert * paquete, t_config * config)
{
	return verificar_tamanio_value(paquete->value_long,config);
}

bool verificar_espacio_memoria()
{
	return tamanio_memoria_ocupada < max_paginas_disponibles;
}

bool verificar_tamanio_value(uint32_t * value_long,t_config * config)
{
	int tam_max_value = config_get_int_value(config, "TAM_MAX_VALUE");

	return ((int) value_long) < tam_max_value;
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
	t_list * lista_paginas_sin_modificar = paginas_sin_modificar(nombre_tabla);

	int size = list_size(lista_paginas_sin_modificar);

//	list_destroy_and_destroy_elements(lista_paginas_sin_modificar,eliminar_filter);

	return size>0;
}

t_list* paginas_modificadas(t_list * lista)
{
	bool _modificadas(t_pagina_completa* elemento){return elemento->flag == 1;}

	int cantidad_paginas_modificadas = list_count_satisfying(lista,_modificadas);


	t_list* lista_modificadas = list_create();
	int i=0;
	while(i<cantidad_paginas_modificadas){
		t_pagina_completa * pagina_modificada = list_remove_by_condition(lista,_modificadas);


		tamanio_memoria_ocupada -= 1;

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


void reemplazar_pagina(char* nombre_tabla, t_pagina_completa* pagina_completa, t_config* config)
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

	int max_memoria = config_get_int_value(config, "TAM_MEM");

	//printf("\nPagina que se remplazo: %d", pagina_A_Reemplazar->pagina->valor_key);

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


void set_tamanio_memoria(int max_value_FS, t_config * config)
{
	int tam_max_memoria = config_get_int_value(config, "TAM_MEM");
	max_paginas_disponibles = 	tam_max_memoria / max_value_FS;
	printf("\nCantidad MAXIMA de paginas disponibles: %d",max_paginas_disponibles);
}

void mostrar_tamanio_memoria_ocupada()
{
	printf("\n Cantidad paginas ocupadas: %d", tamanio_memoria_ocupada);
}

void startup_memoria()
{
	tabla_segmentos = dictionary_create();
//	tabla_particiones = list_create();

	tamanio_memoria_ocupada = 0;

	mostrar_tamanio_memoria_ocupada();


	//
//	t_list* tabla_paginas0 = crear_tabla_paginas("TABLA0","SC",4);
//	t_list* tabla_paginas1 = crear_tabla_paginas("TABLA1","SC",4);
//	t_list* tabla_paginas2 = crear_tabla_paginas("TABLA2","SC",7);
//
//	t_pagina* pagina0 = crear_pagina(0, "CERO", 12);
//	t_pagina* pagina1 = crear_pagina(1, "UNO", 34);
//	t_pagina* pagina2 = crear_pagina(2, "DOS", 56);
//	t_pagina* pagina3 = crear_pagina(3, "TRES", 178);
//	t_pagina* pagina4 = crear_pagina(4, "CUATRO", 90);
//
//	t_pagina_completa* pagina_completa0 = crear_pagina_completa(pagina0);
//	t_pagina_completa* pagina_completa1 = crear_pagina_completa(pagina1);
//	t_pagina_completa* pagina_completa2 = crear_pagina_completa(pagina2);
//	t_pagina_completa* pagina_completa3 = crear_pagina_completa(pagina3);
//	t_pagina_completa* pagina_completa4 = crear_pagina_completa(pagina4);
//
//	agregar_pagina("TABLA0", pagina_completa0);
//	agregar_pagina("TABLA0", pagina_completa3);
//	agregar_pagina("TABLA0", pagina_completa4);
//	agregar_pagina("TABLA1", pagina_completa1);
//	agregar_pagina("TABLA2", pagina_completa2);
//


//	int tamanio0 = sizeof(int) + sizeof(long long) + sizeof(uint16_t) + strlen(pagina_completa0->pagina->value);
//	int tamanio3 = sizeof(int) + sizeof(long long) + sizeof(uint16_t) + strlen(pagina_completa3->pagina->value);
//	int tamanio4 = sizeof(int) + sizeof(long long) + sizeof(uint16_t) + strlen(pagina_completa4->pagina->value);

//	int x = sizeof(int);
//	int y = sizeof(pagina_completa0->pagina->valor_key);
//	printf("\ntamanio sizeof paginacompleta: %d",x); NO DAN IGUAL
//	printf("\ntamanio suma a extraer: %d",tamanio0+tamanio3+tamanio4);
//
//	pagina_completa0->flag = 1;
//	pagina_completa3->flag = 1;
//	pagina_completa4->flag = 1;
//
	mostrar_tabla_segmentos();
//	mostrar_tamanio_memoria_ocupada();
}
