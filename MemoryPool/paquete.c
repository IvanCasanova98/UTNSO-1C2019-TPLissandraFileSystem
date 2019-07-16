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

t_paquete_create* crear_paquete_create(char* nombre_tabla, char* consistencia, int particiones, int tiempo_compactacion)
{
	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_consistencia = strlen(consistencia)+1;
	t_paquete_create* paquete = malloc(tamanio_tabla + tamanio_consistencia + sizeof(uint32_t)*2 + sizeof(int)*2);

	paquete->nombre_tabla= nombre_tabla;
	paquete->consistencia = consistencia;
	paquete->particiones= particiones;
	paquete->tiempo_compactacion= tiempo_compactacion;
	paquete->nombre_tabla_long = tamanio_tabla;
	paquete->consistencia_long = tamanio_consistencia;

	return paquete;
}

t_paquete_drop* crear_paquete_drop(char *nombre_tabla)
{
	uint32_t tamanio_tabla= strlen(nombre_tabla)+1;
	t_paquete_drop* paquete= malloc(tamanio_tabla + sizeof(int));

	paquete->nombre_tabla = nombre_tabla;
	paquete->nombre_tabla_long = tamanio_tabla;

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
	parametros = strtok(NULL, "\"");
	value = parametros;
	parametros = strtok(NULL, " ");
	timestamp = get_timestamp(parametros);


	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);

	loggear_paquete_insert(paquete, logger);

	return paquete;
}

long long get_timestamp(char* parametros){
	long long valor;
	if (parametros==NULL) {
		struct timeval te;
		gettimeofday(&te, NULL); // get current time
		valor = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {valor = atoll(parametros);}

	return valor;
}


t_paquete_create* paquete_create(char* parametros, t_log* logger)
{
	char* nombre_tabla;
	char* consistencia;
	int particiones;
	int tiempo_compactacion;

	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	consistencia = parametros;
	parametros = strtok(NULL, " ");
	particiones = atoi(parametros);
	parametros = strtok(NULL, " ");
	tiempo_compactacion = atoi(parametros);

	t_paquete_create* paquete = crear_paquete_create(nombre_tabla, consistencia, particiones, tiempo_compactacion);

	loggear_paquete_create(paquete, logger);

	return paquete;
}

t_paquete_drop* paquete_drop(char *parametros, t_log* logger){

	char* nombre_tabla;
	parametros= strtok(NULL, " ");
	if(parametros==NULL){
//		faltaTabla();
		return NULL;
	}
	nombre_tabla = parametros;
	t_paquete_drop* paquete = crear_paquete_drop(nombre_tabla);
//	loggear_request_drop(paquete);

	return paquete;
}
