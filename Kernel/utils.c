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

	//printf("\nCONECTADO A LA MEMORIA %d", numero_memoria); //LOGGEAR, NO PRINTEAR
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
		puts("CREATE");
		return 0;
	}
	else if (strcmp(parametros, "DROP")==0) {
		puts("DROP");
		return 1;
	}
	else if (strcmp(parametros, "DESCRIBE")==0) {
		puts("DESCRIBE");
		return 2;
	}
	else if (strcmp(parametros, "SELECT")==0) {
		puts("SELECT");
		return 3;
	}
	else if (strcmp(parametros, "INSERT")==0) {
		puts("INSERT");
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

//---------------------------------------------VALIDACION GENERAL

bool verificar_request(int cod_ingresado, char** vector_parametros) //DEVUELVE FALSE CON ERROR
{
	switch(cod_ingresado)
	{
	case 0:
		return verif_create(vector_parametros);
		break;
	case 1:
		return verif_drop(vector_parametros);
		break;
	case 3:
		return verif_select(vector_parametros);
		break;
	case 4:
		return verif_insert(vector_parametros);
		break;
	default:
		return true;
		break;
	}
}

bool validar_numero(char* parametro){
	for(int i=0;i<string_length(parametro);i++){
		if(!isdigit(parametro[i])) return false;
	}
	return true;
}

//CREATE
bool verif_create(char** vector_parametros)
{
	if(vector_parametros[1]==NULL)
	{
		falta_tabla();
		return false;
	}
	else if(vector_parametros[2]==NULL)
	{
		falta_consistencia();
		return false;
	}
	else if(!(strcmp(vector_parametros[2],"SC") || strcmp(vector_parametros[2],"SHC") || strcmp(vector_parametros[2],"EC")))
	{
		falta_consistencia();
		return false;
	}
	else if(vector_parametros[3]==NULL || !validar_numero(vector_parametros[3]) || !strcmp(vector_parametros[3],"0"))
	{
		falta_particiones();
		return false;
	}
	else if(vector_parametros[4]==NULL || !validar_numero(vector_parametros[4]) || !strcmp(vector_parametros[4],"0"))
	{
		falta_tiempo_compactacion();
		return false;
	}
	else{return true;}
}

//DROP
bool verif_drop(char** vector_parametros)
{
	return existe_tabla(vector_parametros[1]);
}


//SELECT
bool verif_select(char** vector_parametros)
{
	if(vector_parametros[1]==NULL)
	{
		falta_tabla();
		return false;
	}
	else if(!existe_tabla(vector_parametros[1]))
	{
		return false;
	}
	else if(vector_parametros[2]==NULL || !validar_numero(vector_parametros[2])){
		falta_key();
		return false;
	}
	else{return true;}
}

//INSERT
bool verif_insert(char** vector_parametros)
{
	char* value_completo;

	if(vector_parametros[1]==NULL)
	{
		falta_tabla();
		return false;
	}
	else if(vector_parametros[2]==NULL || !validar_numero(vector_parametros[2]))
	{
		falta_key();
		return false;
	}
	else if(vector_parametros[3]==NULL)
	{
		falta_value();
		return false;
	}
	else
	{
		value_completo = concatenar_value(vector_parametros);
		if((string_starts_with(value_completo, "\"") && string_ends_with(value_completo, "\"")))
		{
			return true;
		}
		else
		{
			falta_value();
			return false;
		}
	}
}

//---------------------------------------------LOGGER DE VALIDACION
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





