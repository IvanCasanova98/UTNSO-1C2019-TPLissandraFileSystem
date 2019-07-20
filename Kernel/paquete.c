#include "paquete.h"

//FALTA SLEEPS, AGREGAR QUE SI TOCO ENTER NO ANDE, Y QUE ANDE DESCRIBE POST CREATE BIEN
//HAGO INSERT Y ENTRA POR EL ELSE, NO EXISTE LA TABLA

//---------------------CREAR PAQUETES

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

t_paquete_select* selectf(char** vector_parametros)
{
	uint16_t valor_key;;

	if(vector_parametros[1]==NULL)
	{
		falta_tabla();
		return NULL;
	}

	if(!existe_tabla(vector_parametros[1]))
	{
		return NULL;
	}

	if(vector_parametros[2]==NULL || !validar_numero(vector_parametros[2])){
		falta_key();
		return NULL;
	}
	valor_key = atoi(vector_parametros[2]);

	t_paquete_select* paquete = crear_paquete_select(vector_parametros[1], valor_key);

	loggear_paquete_select(paquete);

	return paquete;
}

t_paquete_insert* insert(char** vector_parametros){

	uint16_t valor_key;

	char* value_completo;
	long long timestamp=0;


	if(vector_parametros[1]==NULL)
	{
		falta_tabla();
		return NULL;
	}

	if(vector_parametros[2]==NULL || !validar_numero(vector_parametros[2]))
	{
		falta_key();
		return NULL;
	}
	valor_key = atoi(vector_parametros[2]);


	if(vector_parametros[3]==NULL)
	{
		falta_value();
		return NULL;
	}
	else
	{
		value_completo = concatenar_value(vector_parametros);
		if((string_starts_with(value_completo, "\"") && string_ends_with(value_completo, "\"")))
		{
			value_completo = strtok(value_completo, "\"");
		}
		else
		{
			falta_value();
			return NULL;
		}
	}

	timestamp = get_timestamp();

	t_paquete_insert* paquete = crear_paquete_insert(vector_parametros[1], valor_key, value_completo, timestamp);
	loggear_paquete_insert(paquete);

	return paquete;
}

long long get_timestamp(){
	long long valor;

	struct timeval te;
	gettimeofday(&te, NULL);
	valor = te.tv_sec*1000LL + te.tv_usec/1000;

	return valor;
}

t_paquete_create* create(char** vector_parametros) //ARREGLAR VERIFICACIONES POR "FALTAN DATOS"
{
	int particiones;
	int tiempo_compactacion;


	if(vector_parametros[1]==NULL)
	{
		falta_tabla();
		return NULL;
	}

	if(vector_parametros[2]==NULL)
	{
		falta_consistencia();
		return NULL;
	}
	if(!strcmp(vector_parametros[2],"SC") || !strcmp(vector_parametros[2],"SHC") || !strcmp(vector_parametros[2],"EC")){}
	else
	{
		falta_consistencia();
		return NULL;
	}

	if(vector_parametros[3]==NULL || !validar_numero(vector_parametros[3]) || !strcmp(vector_parametros[3],"0"))
	{
		falta_particiones();
		return NULL;
	}
	particiones = atoi(vector_parametros[3]);


	if(vector_parametros[4]==NULL || !validar_numero(vector_parametros[4]) || !strcmp(vector_parametros[4],"0"))
	{
		falta_tiempo_compactacion();
		return NULL;
	}
	tiempo_compactacion = atoi(vector_parametros[4]);

	t_paquete_create* paquete = crear_paquete_create(vector_parametros[1], vector_parametros[2], particiones, tiempo_compactacion);

	loggear_paquete_create(paquete);

	return paquete;
}

void describe(int conexion, char* parametros){
	int cod_ingresado = 2;
	send(conexion, &cod_ingresado, sizeof(int), 0);

	if (parametros==NULL)
	{
		int tamanio = strlen("ALL")+1;
		char* buffer = "ALL";
		send(conexion, &tamanio, sizeof(int), 0);
		send(conexion, buffer, tamanio, 0);
		//logger_describe_all();
	}
	else
	{
		int tamanio = strlen(parametros)+1;
		send(conexion, &tamanio, sizeof(int), 0);
		send(conexion, parametros, tamanio, 0);
		//logger_describe_tabla(parametros);
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
    log_info(logger,"INSERT  %s  %d  %s\n", paquete->nombre_tabla, paquete->valor_key, paquete->value);
	log_destroy(logger);
}

void loggear_paquete_create(t_paquete_create* paquete)
{
	t_log* logger = iniciar_logger();
	log_info(logger, "CREATE %s %s %d %d\n", paquete->nombre_tabla, paquete->consistencia, paquete->particiones, paquete->tiempo_compactacion);
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
