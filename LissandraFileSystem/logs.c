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


// WARNINGS DE ERRORES

void LaTablaNoExiste(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL INSERTAR %lli;%d;%s en %s LA TABLA NO EXISTE",timestamp,key,value,nombreTabla );
	log_destroy(logger);
}

void LaTablaYaExiste(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL CREAR %s LA TABLA YA EXISTE",nombreTabla);
	log_destroy(logger);
}


void LogDumpeo(){
	t_log* logger=iniciar_logger();
	log_info(logger,"REALIZANDO DUMP");
	log_destroy(logger);
}
