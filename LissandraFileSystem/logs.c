/*
 * CREO ESTE .c para generalizar todos los logs aca
 *
 *
 *
 */
#include "logs.h"

void LogearInsert(char* timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"INSERTANDO %lli;%d;%s en %s",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}

void LogearCreate(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"CREANDO TABLA %s",nombreTabla);
	log_destroy(logger);
}
void LogearSelect(char* timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"BUSCANDO  %lli;%d;%s en %s",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}


// WARNINGS DE ERRORES

void LaTablaNoExiste(char* timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL INSERTAR %lli;%d;%s en %s LA TABLA NO EXISTE",timestamp,key,value,nombreTabla );
	log_destroy(logger);
}

void LaTablaYaExiste(char* timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL CREAR %s LA TABLA YA EXISTE",nombreTabla);
	log_destroy(logger);
}
