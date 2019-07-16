#include "paquete.h"

//FALTA SLEEPS, AGREGAR QUE SI TOCO ENTER NO ANDE, Y QUE ANDE DESCRIBE POST CREATE BIEN
//HAGO INSERT Y ENTRA POR EL ELSE, NO EXISTE LA TABLA

//---------------------CREAR PAQUETES

//*******************************************************************
t_paquete_drop* crear_paquete_drop(char *nombre_tabla)
{
	uint32_t tamanio_tabla= strlen(nombre_tabla)+1;
	t_paquete_drop* paquete= malloc(tamanio_tabla + sizeof(int));

	paquete->nombre_tabla = nombre_tabla;
	paquete->nombre_tabla_long = tamanio_tabla;

	return paquete;
}
//*******************************************************************


t_paquete_select* crear_paquete_select(char* nombre_tabla, uint16_t valor_key) //Agregado
{
	uint32_t tamaniotabla = strlen(nombre_tabla)+1;
	t_paquete_select* paquete = malloc(sizeof(int)+sizeof(uint16_t)+tamaniotabla);

	paquete->nombre_tabla= nombre_tabla;
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;
	free(paquete);

}

t_paquete_insert* crear_paquete_insert(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp) //Agregado
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
	free(paquete);
}

t_paquete_create* crear_paquete_create(char* nombre_tabla, char* consistencia, int particiones, int tiempo_compactacion) //Agregado
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
	free(paquete);
}

//---------------------ARMAR PAQUETES

t_paquete_select* selectf(char* parametros){


	uint16_t valor_key;
	char* nombre_tabla;

	parametros= strtok(NULL, " ");

	if(parametros==NULL)
	{
		falta_tabla();
		return NULL;
	}
	nombre_tabla = parametros;

	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros)){
		falta_key();
		return NULL;
	}
	valor_key = atoi(parametros);

	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);

	loggear_paquete_select(paquete);

	return paquete;
}

t_paquete_insert* insert(char* parametros){

	uint16_t valor_key;
	char* nombre_tabla;
	char* value;
	long long timestamp=0;

	parametros= strtok(NULL, " ");
	if(parametros==NULL)
	{
		falta_tabla();
		return NULL;
	}
	nombre_tabla = parametros;

	parametros = strtok(NULL, " ");
//	printf("\nvalor key %s",parametros);
	if(parametros==NULL || !validar_numero(parametros))
	{
		falta_key();
		return NULL;
	}
	valor_key = atoi(parametros);

	parametros = strtok(NULL, "\"");
	if(parametros==NULL) //falta verificar que value vaya si o si con comillas
	{
		falta_value();
		return NULL;
	}
	value = parametros;
	parametros = strtok(NULL, " ");
	timestamp = get_timestamp(parametros);


	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);
	loggear_paquete_insert(paquete);

	return paquete;
}

long long get_timestamp(char* parametros){
	long long valor;
	if (parametros==NULL) {
		struct timeval te;
		gettimeofday(&te, NULL);
		valor = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {valor = atoll(parametros);}

	return valor;
}

t_paquete_create* create(char* parametros)
{
	char* nombre_tabla;
	char* consistencia;
	int particiones;
	int tiempo_compactacion;

	parametros= strtok(NULL, " ");
	if(parametros==NULL)
	{
		falta_tabla();
		return NULL;
	}
	nombre_tabla = parametros;

	parametros = strtok(NULL, " ");
	if(!strcmp(parametros,"SC") || !strcmp(parametros,"SHC") || !strcmp(parametros,"EC"))
	{
		consistencia = parametros;
	}
	else
	{
		falta_consistencia();
		return NULL;
	}


	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros) || !strcmp(parametros,"0"))
	{
		falta_particiones();
		return NULL;
	}
	particiones = atoi(parametros);

	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros) || !strcmp(parametros,"0"))
	{
		falta_tiempo_compactacion();
		return NULL;
	}
	tiempo_compactacion = atoi(parametros);

	t_paquete_create* paquete = crear_paquete_create(nombre_tabla, consistencia, particiones, tiempo_compactacion);

	loggear_paquete_create(paquete);

	return paquete;
}

void describe(int conexion, char* parametros){ //parametros es el nombre de la tabla a describir
	int cod_ingresado = 2;
	send(conexion, &cod_ingresado, sizeof(int), 0);

	if (parametros==NULL)
	{
		int tamanio = strlen("ALL")+1;
		char* buffer = "ALL";
		send(conexion, &tamanio, sizeof(int), 0);
		send(conexion, buffer, tamanio, 0);
		logger_describe_all();
	}
	else
	{
		int tamanio = strlen(parametros)+1;
		send(conexion, &tamanio, sizeof(int), 0);
		send(conexion, parametros, tamanio, 0);
		logger_describe_tabla(parametros);
	}

	deserealizar_metadata(conexion);
}

//-------------------------LOGGERS

void loggear_paquete_select(t_paquete_select* paquete){ //FALTA PASAR LOOGER

	t_log* logger = iniciar_logger();
	//log_info(logger, "SELECT\nNombre tabla: %s\nValor KEY   : %d", paquete->nombre_tabla,paquete->valor_key);
	log_info(logger, "SELECT  %s  %d", paquete->nombre_tabla,paquete->valor_key);

	log_destroy(logger);
}

void loggear_paquete_insert(t_paquete_insert* paquete){

	t_log* logger = iniciar_logger();
	//log_info(logger, "INSERT\nNombre tabla: %s\nValor KEY   : %d\nValue       : %s", paquete->nombre_tabla, paquete->valor_key, paquete->value);
    log_info(logger,"INSERT  %s  %d  %s", paquete->nombre_tabla, paquete->valor_key, paquete->value);
	log_destroy(logger);
}

void loggear_paquete_create(t_paquete_create* paquete)
{
	t_log* logger = iniciar_logger();
	log_info(logger, "CREATE %s %s %d %d", paquete->nombre_tabla, paquete->consistencia, paquete->particiones, paquete->tiempo_compactacion);
    log_destroy(logger);
}
void logger_describe_all(){
	t_log* logger = iniciar_logger();
	log_info(logger, "DESCRIBE ALL\n");
    log_destroy(logger);
}
void logger_describe_tabla(char* nombre_tabla){
	t_log* logger = iniciar_logger();
	log_info(logger, "DESCRIBE %s \n", nombre_tabla);
    log_destroy(logger);
}









//----------------------------------------------------ELIMINAR PAQUETE

//void eliminar_paquete_select(t_paquete_select* paquete)
//{
//    free(paquete->buffer->stream);
//    free(paquete->buffer);
//    free(paquete);
//}

//void eliminar_paquete_insert(t_paquete_insert* paquete)
//{
//    free(paquete->buffer->stream);
//    free(paquete->buffer);
//    free(paquete);
//}
