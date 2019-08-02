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

//	agregar_consistencia(0, "SC");
//	agregar_consistencia(1, "EC");
//	agregar_consistencia(1, "SHC");
//	agregar_consistencia(1, "SHC");
//	agregar_consistencia(2, "SHC");
//	agregar_consistencia(2, "EC");
//	agregar_consistencia(3, "EC");
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
	if(cant_elementos<=0){
		if(cant_elementos==0){
			t_log* logger=iniciar_logger();
			log_warning(logger,"FILE SYSTEM NO TIENE TABLAS");
			log_destroy(logger);
		}
		if(cant_elementos==-1){
			t_log* logger=iniciar_logger();
			log_warning(logger,"FILE SYSTEM CAIDO");
			log_destroy(logger);
		}
	}
	else
	{
	int i=0;
		while(i<cant_elementos)
		{
			t_metadata* nodo_metadata = deserealizar_nodo(socket);
//			loggearMetadataTablas(nodo_metadata);
			agregar_datos(nodo_metadata->nombre_tabla, nodo_metadata->consistencia);
			i++;
		}
	}

//	dictionary_iterator(tabla_metadata,imprimir_diccionario);
}

void remover_memoria_consistencia(SEED * memoria)
{
	bool _boolean(int numero_memoria){return numero_memoria == memoria->NUMBER;}

	for(int i = 0; i<3 ; i++)
	{
		list_remove_by_condition(vector_consistencia[i], _boolean);
	}
}

void journal_memorias(t_log* logger)
{
	int i = 0;
	int cod_ingresado = 5;
	int size = list_size(lista_seeds);

	while(i<size)
	{
		SEED * memoria_i = list_get(lista_seeds,i);

		char * puerto_char = string_itoa(memoria_i->PUERTO);
		char ** ip_sin_comillas = string_split(memoria_i->IP,"\"");

		int conexion = crear_conexion(ip_sin_comillas[0],puerto_char);

		if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");

		close(conexion);
		i++;
	}
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

t_list* get_pool(int cons_ingresada) //FUNCION QUE DEVUELVE SOLO LA LISTA DE CONSISTENCIAS ESPECIFICAS
{
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
