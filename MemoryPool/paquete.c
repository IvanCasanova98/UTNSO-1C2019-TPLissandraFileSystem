#include "paquete.h"

//---------------------CREAR PAQUETES

t_paquete_select* crear_paquete_select(char* nombretabla, uint16_t valor_key)
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_select* paquete = malloc(sizeof(int)+sizeof(uint16_t)+tamaniotabla);

	paquete->nombre_tabla= nombretabla;
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;
}

t_paquete_insert* crear_paquete_insert(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp)
{
	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_value = strlen(value)+1;
	t_paquete_insert* paquete = malloc(sizeof(long long) + sizeof(int)*2 + tamanio_tabla + tamanio_value + sizeof(uint16_t));

	paquete->nombre_tabla= nombre_tabla;
	paquete->value = value;
	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;
	paquete->nombre_tabla_long= tamanio_tabla;
	paquete->value_long= tamanio_value;

	return paquete;
}

//---------------------ARMAR PAQUETES

t_paquete_select* paquete_select(char* parametros, t_log* logger)
{
	uint16_t valor_key;
	char* nombre_tabla;

	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	valor_key = atoi(parametros);

	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);

	loggear_paquete_select(paquete, logger);

	return paquete;
}

t_paquete_insert* paquete_insert(char* parametros, t_log* logger)
{
	uint16_t valor_key;
	char* nombre_tabla;
	char* value;
	long long timestamp=0;

	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	valor_key = atoi(parametros);
	parametros = strtok(NULL, " ");
	value = parametros;

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);

	loggear_paquete_insert(paquete, logger);

	return paquete;
}
