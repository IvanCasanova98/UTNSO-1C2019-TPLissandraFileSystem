#include "utils.h"

//---------------------------CALCULAR TAMAÑO DE CHAR**------------------
int tamanio_array(char** array)
{
	int tamanio=0;
	int i=0;
	while(array[i]!=NULL)
	{
		tamanio+= sizeof(int) + strlen(array[i]) + 5; //Por qué hay que agregarle 5 para que ande bien.
		i++;
	}
	return tamanio;
}

int tamanio_array_puerto(char** array)
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
	log_info(logger, "SELECT:\nNombre tabla: %s\nValor KEY   : %d", paquete->nombre_tabla,paquete->valor_key);
}

void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger)
{
	log_info(logger, "INSERT:\nNombre tabla: %s\nValor KEY   : %d\nValue       : %s\nTimestamp   : %lld", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
}

void loggear_paquete_create(t_paquete_create* paquete, t_log* logger)
{
	log_info(logger, "CREATE:\nNombre tabla       : %s\nConsistencia       : %s\nParticiones        : %d\nTiempo compactacion: %d", paquete->nombre_tabla, paquete->consistencia, paquete->particiones, paquete->tiempo_compactacion);
}
