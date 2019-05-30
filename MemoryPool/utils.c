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
	log_info(logger, "NUEVO PAQUETE SELECT CREADO\nNombre tabla: %s\nValor KEY   : %d", paquete->nombre_tabla,paquete->valor_key);
}

void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger)
{
	log_info(logger, "NUEVO PAQUETE INSERT CREADO\nNombre tabla: %s\nValor KEY   : %d\nValue       : %s\n timestamp   : %lld", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
}
