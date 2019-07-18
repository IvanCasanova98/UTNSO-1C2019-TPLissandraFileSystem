#include "memorias.h"

t_dictionary* tabla_metadata;
t_list* vector_consistencia[3];

void startup_diccionario()
{
	tabla_metadata = dictionary_create();

	t_list* lista_sc = list_create();
	vector_consistencia[0] = lista_sc;

	t_list* lista_shc = list_create();
	vector_consistencia[1] = lista_shc;

	t_list* lista_ec = list_create();
	vector_consistencia[2] = lista_ec;

	agregar_consistencia(0, "SC");
	agregar_consistencia(0, "EC");
	agregar_consistencia(1, "SHC");
	agregar_consistencia(2, "EC");
	agregar_consistencia(3, "EC");
}

void agregar_datos(char* nombre_tabla, char* consistencia){

	dictionary_put(tabla_metadata,nombre_tabla,consistencia);
}

bool existe_tabla(char* nombre_tabla){
	return dictionary_has_key(tabla_metadata, nombre_tabla);
}

void deserealizar_metadata(int socket){
	int cant_elementos;
	recv(socket,&cant_elementos,sizeof(int),MSG_WAITALL);

	int i=0;
	while(i<cant_elementos){
		t_metadata* nodo_metadata = deserealizar_nodo(socket);
		agregar_datos(nodo_metadata->nombre_tabla, nodo_metadata->consistencia);
		i++;
	}

//	dictionary_iterator(tabla_metadata,imprimir_diccionario);
}

//---------------------------CONSISTENCIAS
void agregar_consistencia(int memoria, char* consistencia)
{
	int cons_ingresada = consistencia_ingresada(consistencia);
	switch(cons_ingresada)
	{
	case SC:
		//hay que preguntar si ya hay alguien con SC para informar error?
		list_clean(vector_consistencia[0]);
		list_add(vector_consistencia[0],memoria);
		break;
	case SHC:;
		list_add(vector_consistencia[1],memoria);
		break;
	case EC:
		list_add(vector_consistencia[2],memoria);
		break;
	default:
		printf("\nconsistencia erronea");
		break;
	}
//	imprimir_consistencias();
}

bool verificar_consistencias()
{
	return (list_is_empty(vector_consistencia[0]) && list_is_empty(vector_consistencia[1]) && list_is_empty(vector_consistencia[2]));
}

t_list* get_pool(char* tipo_consistencia) //FUNCION QUE DEVUELVE SOLO LA LISTA DE CONSISTENCIAS ESPECIFICAS
{
	int cons_ingresada = consistencia_ingresada(tipo_consistencia);
	switch(cons_ingresada)
	{
	case SC:
		return vector_consistencia[0];
		break;
	case SHC:;
		return vector_consistencia[1];
		break;
	case EC:
		return vector_consistencia[2];
		break;
	default:
		printf("\nconsistencia erronea");
		return NULL; //MM.. nose eso
		break;
	}
}

char * get_consistencia(char * nombre_tabla) //FUNCION QUE DEVUELVE LA CONSISTENCIA DE 1 TABLA ESPECIFICA DE LA METADATA
{
	return dictionary_get(tabla_metadata,nombre_tabla);
}


//-------------------------UTILIDADES
void imprimir_consistencias()
{
	for(int i = 0; i<3; i++)
	{
		if(!list_is_empty(vector_consistencia[i]))
		{
			void _imprimir(int memoria){printf("\nCONSISTENCIA: %d MEMORIA: %d", i, memoria);}
			list_iterate(vector_consistencia[i], _imprimir);
		}
	}
}

void imprimir_diccionario(char* nombre_tabla,char*consistencia){
	printf("\nNombre: %s   Consistencia: %s\n", nombre_tabla,consistencia);
}
