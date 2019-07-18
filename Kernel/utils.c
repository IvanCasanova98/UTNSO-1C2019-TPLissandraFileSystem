#include "utils.h"

//----------------------------ARCHIVOS LOGGER Y CONFIG

t_log* iniciar_logger()
{
	return log_create("Kernel.log", "Kernel", 0, LOG_LEVEL_INFO);
}

t_config* leer_config()
{
	return config_create("Kernel.config");
}

//---------------------------RECIBIR NUMERO DE MEMORIA

void recibir_numero_memoria(int conexion)
{
	int numero_memoria;
	recv(conexion, &numero_memoria, sizeof(int), 0);

	printf("\nCONECTADO A LA MEMORIA %d", numero_memoria); //LOGGEAR, NO PRINTEAR
}

//-----------------------------------------RETARDOS

void retardo_ejecucion(t_config* config)
{
	int retardo_ejecucion = config_get_int_value(config, "SLEEP_EJECUCION");
	usleep(retardo_ejecucion);
}

//-------------------------------------------------------
int codigo_ingresado(char* parametros)
{
	if (strcmp(parametros, "CREATE")==0) {
		return 0;
	}
	else if (strcmp(parametros, "DROP")==0) {
		return 1;
	}
	else if (strcmp(parametros, "DESCRIBE")==0) {
		return 2;
	}
	else if (strcmp(parametros, "SELECT")==0) {
		return 3;
	}
	else if (strcmp(parametros, "INSERT")==0) {
		return 4;
	}
	else if (strcmp(parametros, "JOURNAL")==0) {
		return 5;
	}
	else if (strcmp(parametros, "RUN")==0) {
		return 6;
	}
	else if (strcmp(parametros, "ADD")==0) {
		return 7;
	}
	else{
		return 8;
	}
}

int consistencia_ingresada(char* parametros)
{
	if (strcmp(parametros, "SC")==0) {
		return 0;
	}
	else if (strcmp(parametros, "SHC")==0) {
		return 1;
	}
	else if (strcmp(parametros, "EC")==0) {
		return 2;
	}
	else {return 3;}
}

int numero_random(int max)
{
	srand (getpid());
	int rnd = rand() % max;
	return rnd;
}

char * concatenar_value(char ** vector)
{
	int i=3;
	char * value=string_new();
	while(vector[i]!=NULL)
	{
		string_append(&value,vector[i]);
		string_append(&value," ");
		i++;
	}
	string_trim_right(&value);

	return value;

}
//---------------------------------------------FUNCIONES PARA VALIDAR

bool validar_numero(char* parametro){
	for(int i=0;i<string_length(parametro);i++){
		if(!isdigit(parametro[i])) return false;
	}
	return true;
}

//bool validar_nombre_alfanumerico(char* parametro){
//	int x=0;
//	for(int i=0;i<string_length(parametro);i++){
//		if(isdigit(parametro[i])) x++;
//	}
//	if(x==string_length(parametro)){
//		return true;
//	}else{
//		return false;
//	}
//}

void falta_tabla(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO PARAMETRO NOMBRETABLA CORRECTAMENTE\n");
	log_destroy(logger);
}

void falta_key(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO PARAMETRO KEY CORRECTAMENTE\n");
	log_destroy(logger);
}

void falta_value(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO PARAMETRO VALUE CORRECTAMENTE\n");
	log_destroy(logger);
}


void falta_consistencia(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO PARAMETRO CONSISTENCIA CORRECTAMENTE\n");
	log_destroy(logger);
}

void falta_particiones(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO PARAMETRO PARTICIONES CORRECTAMENTE\n");
	log_destroy(logger);
}
void falta_tiempo_compactacion(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO PARAMETRO TIEMPO DE COMPACTACION CORRECTAMENTE\n");
	log_destroy(logger);
}

void describe_invalido(){

	t_log* logger=iniciar_logger();
	log_error(logger, "ERROR NO SE INGRESO EL DESCRIBE CORRECTAMENTE\n");
	log_destroy(logger);
}





