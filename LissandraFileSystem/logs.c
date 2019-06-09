/*
 * CREO ESTE .c para generalizar todos los logs aca
 *
 *
 *
 */
#include "logs.h"

void LogearInsert(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"INSERTANDO %lli;%d;%s en %s\n",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}

void LogearDropCorrecto(char* nombreTabla)
{
	t_log* logger=iniciar_logger();
	log_info(logger,"BORRANDO %s\n",nombreTabla);
	log_destroy(logger);
}

void LogearDropFallido(char* nombreTabla)
{
	t_log* logger=iniciar_logger();
	log_info(logger,"NO EXISTE %s\n",nombreTabla);
	log_destroy(logger);
}

void LogearCreate(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"CREANDO TABLA %s\n",nombreTabla);
	log_destroy(logger);
}
void LogearSelect(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"BUSCANDO  %lli;%d;%s en %s\n",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}

void valueExiste(char*nombreTabla,uint16_t key,char* value){

	t_log* logger=iniciar_logger();
	log_info(logger,"EL VALOR DE LA KEY %d en la TABLA %s ES %s",key,nombreTabla,value);
	log_destroy(logger);
}

// WARNINGS DE FALTAN DATOS
void faltaTabla(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO NOMBRETABLA CORRECTAMENTE");
	log_destroy(logger);
}

void faltaKey(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO KEY CORRECTAMENTE");
	log_destroy(logger);
}

void faltaValue(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO VALUE CORRECTAMENTE");
	log_destroy(logger);
}

void faltaTimestamp(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO TIMESTAMP CORRECTAMENTE");
	log_destroy(logger);
}

void faltaConsistencia(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO CONSISTENCIA CORRECTAMENTE");
	log_destroy(logger);
}

void faltaParticiones(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO PARTICIONES CORRECTAMENTE");
	log_destroy(logger);
}

void faltaTiempo_Compactacion(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR NO SE INGRESO PARAMETRO TIEMPO_COMPACTACION CORRECTAMENTE");
	log_destroy(logger);
}


// WARNINGS DE ERRORES

void LaTablaNoExisteSelect(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL BUSCAR KEY LA TABLA %s NO EXISTE",nombreTabla );
	log_destroy(logger);
}


void LaTablaNoExiste(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL INSERTAR %lli;%d;%s en %s LA TABLA NO EXISTE",timestamp,key,value,nombreTabla );
	log_destroy(logger);
}

void LaTablaYaExiste(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL CREAR %s LA TABLA YA EXISTE",nombreTabla);
	log_destroy(logger);
}

void valueNoExiste(char* nombreTabla,int key){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL BUSCAR KEY %d EN %s NO EXISTE REGISTRO",key,nombreTabla);
	log_destroy(logger);
}


void LogDumpeo(){
	t_log* logger=iniciar_logger();
	log_info(logger,"REALIZANDO DUMP");
	log_destroy(logger);
}
