#include "utils.h"


int consistencia_to_int(char* parametros)
{
	if (strcmp(parametros, "SC")==0) {
		return 0;
	}
	else if (strcmp(parametros, "SHC")==0) {
		return 1;
	}
	else if (strcmp(parametros, "EC")==0) {
		return 2;
	}
	else {return 3;}
}


//-----------------------------ENVIO NUMERO DE MEMORIA

void envio_numero_memoria(t_config* config, int conexion)
{
	int numero_memoria =  config_get_int_value(config, "MEMORY_NUMBER");
	send(conexion,&numero_memoria,sizeof(int),0);
}

//---------------------------RETARDO

void retardo_ejecucion(t_config* config)
{
	int retardo_ejecucion = config_get_int_value(config, "SLEEP_EJECUCION");
	sleep(retardo_ejecucion);
}

//---------------------------CALCULAR TAMAÑO DE VECTORES------------------
int tamanio_array(char** array)
{
	int tamanio=0;
	int i=0;


	while(array[i]!=NULL)
	{
		tamanio+= sizeof(int) + strlen(array[i]) + 1; //Por qué hay que agregarle 5 para que ande bien.
		i++;
	}
	return tamanio;
}

int tamanio_array_int(int array[])
{
//	int tamanio=0;
//	int i=0;
//	while(array[i]!=NULL)
//	{
//		tamanio+= sizeof(int); //Por qué hay que agregarle 5 para que ande bien.
//		i++;
//	}
//	return tamanio;
}
//-------------CALCULAR CANTIDAD DE ELEMENTOS DE ARRAY---------------
int cant_elementos_array(char** array)
{
	int i=0;
	while(array[i]!=NULL)
	{
		i++;
	}
	return i;
}

//------------------------ENVIO DE ERRORES---------------------------

void enviar_select_error(int cliente)
{
//	t_pagina* pagina_encontrada = buscar_pagina(paquete -> nombre_tabla, paquete -> valor_key);
	char* mensaje_error = "Key no encontrada";

	int bytes = sizeof(int) + strlen(mensaje_error) + 1;
	void* a_enviar = serializar_mensaje(mensaje_error, bytes);
	send(cliente,a_enviar,bytes,0);
}

//----------------------------ARCHIVOS LOGGER Y CONFIG

t_log* iniciar_logger()
{
	return log_create("MemoryPool.log", "MemoryPool", 0, LOG_LEVEL_INFO);
}

t_config* leer_config(char* archivo)
{
	return config_create(archivo);
}

//----------------------------------------------------LOGGEO DE PAQUETES

void loggear_paquete_select(t_paquete_select* paquete, t_log* logger)
{
	log_info(logger, "SELECT %s %d\n", paquete->nombre_tabla,paquete->valor_key);
}

void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger)
{
	log_info(logger, "INSERT %s %d %s %lld\n", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
}

void loggear_paquete_create(t_paquete_create* paquete, t_log* logger)
{
	log_info(logger, "CREATE %s %s %d %d\n", paquete->nombre_tabla, paquete->consistencia, paquete->particiones, paquete->tiempo_compactacion);
}
