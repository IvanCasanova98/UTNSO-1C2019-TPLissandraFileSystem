/*
 * CREO ESTE .c para generalizar todos los logs aca
 *
 *
 *
 */
#include "logs.h"

void LogearInsert(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"INSERTANDO %lli;%d;%s en %s",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}

void logearDescribeTablaEnParticular(char* nombreTablaMayuscula){
	t_log* logger=iniciar_logger();
	log_info(logger,"DESCRIBE DE %s",nombreTablaMayuscula);
	log_destroy(logger);
}

void logearDescribeTodasLasTablas(){
	t_log* logger=iniciar_logger();
	log_info(logger,"DESCRIBE DE TODAS LAS TABLAS");
	log_destroy(logger);
}

void logearDescribeTablaInexistente(char *nombreTablaMayuscula){
	t_log* logger=iniciar_logger();
	log_info(logger,"LA TABLA A DESCRIBIR %s NO EXISTE\n",nombreTablaMayuscula);
	log_destroy(logger);
}

void LogearDropCorrecto(char* nombreTabla)
{
	t_log* logger=iniciar_logger();
	log_info(logger,"SE BORRO %s\n",nombreTabla);
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
	log_info(logger,"EL VALOR DE LA KEY %d en la TABLA %s ES: %s",key,nombreTabla,value);
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

void faltaEnmascararValue(){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR EL PARAMETRO VALUE NO ESTA ENMASCARADO. AGREGAR COMILLAS");
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

void LogCompactacion(char* nombreTabla){
	t_log* logger=iniciar_logger();
	log_info(logger,"REALIZANDO COMPACTACION %s",nombreTabla);
	log_destroy(logger);
}


void LogCompactacionTerminada(char* nombreTabla,int TiempoTranscurrido){
	t_log* logger=iniciar_logger();
	log_info(logger,"SE REALIZO COMPACTACION %s \n TIEMPO TRANSCURRIDO: %d MILISEGUNDOS",nombreTabla,TiempoTranscurrido);
	log_destroy(logger);
}

void LogearBloqueEnUso(int nroBloque,char* nombreTabla){
	t_log* logger=iniciar_logger();
	log_info(logger,"SE OCUPO BLOQUE %d TABLA: %s",nroBloque,nombreTabla);
	log_destroy(logger);
}
void LogearBloqueLiberado(int nroBloque){
	t_log* logger=iniciar_logger();
	log_info(logger,"SE LIBERO BLOQUE %d",nroBloque);
	log_destroy(logger);


}

void LogearNombreTemporal(char* nombreTemporal,int numero,char*nombreTabla){
	t_log* logger=iniciar_logger();
	log_info(logger,"SE CREO TEMPORAL %s%d.tmp  TABLA: %s",nombreTemporal,numero,nombreTabla);
	log_destroy(logger);

}

void LogearNombreTemporalDestruido(char* nombreTemporal,int numero,char*nombreTabla){
	t_log* logger=iniciar_logger();
	log_info(logger,"SE ELIMINO %s%d.tmpc  TABLA: %s",nombreTemporal,numero,nombreTabla);
	log_destroy(logger);
}




