/*
 * Consola.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include"Consola.h"
#include"recibir.h"

void* prenderConsola(void* arg){


	char *lineaRequest = ingresar_request();

	while(string_is_empty(lineaRequest )){
		free(lineaRequest);
		lineaRequest = ingresar_request();

	}

	char* parametros = strtok(lineaRequest, " ");
	string_to_upper(parametros);
	int cod_ingresado = codigo_ingresado((parametros));
	//free(lineaRequest);
	while(1)
	{
		switch(cod_ingresado){
			case 0:;
				t_paquete_create* paquete_create =LeerCreate(parametros) ;
				if(paquete_create==NULL) break;
				APIcreate(paquete_create);
				break;
			case 1:;

				t_paquete_drop* paquete_drop =LeerDrop(parametros);
				if(paquete_drop==NULL) break;
				APIdrop(paquete_drop);
				free(paquete_drop);
				break;
			case 2:;

				t_paquete_describe* paquete_describe = LeerDescribe(parametros);
				if(paquete_describe == NULL){

					t_dictionary* metadatasDeTablasPedidas = APIdescribeTodasLasTablas();
					if(metadatasDeTablasPedidas != NULL)
						imprimirListaMetadatas(metadatasDeTablasPedidas);
					dictionary_destroy_and_destroy_elements(metadatasDeTablasPedidas,free);
//					free(metadatasDeTablasPedidas);

				} else{

					t_dictionary* metadataDeTablaPedida = APIdescribe(paquete_describe);
					if(metadataDeTablaPedida != NULL)
						imprimirListaMetadatas(metadataDeTablaPedida);
					dictionary_destroy_and_destroy_elements(metadataDeTablaPedida,free);
//					free(metadataDeTablaPedida);

				}
				free(paquete_describe);
				break;
			case 3:;

				t_paquete_select* paquete_select =LeerSelect(parametros) ;
				if(paquete_select==NULL) break;
				void* value=APIselect(paquete_select);
				if(value!=NULL){
					//tendria que hacer el send aca
					free(value);
				}

				break;
			case 4:;

				t_paquete_insert* paquete_insert=LeerInsert(parametros);
				if(paquete_insert==NULL) break;
				APIinsert(paquete_insert);
				//en recibir.h tendria que hacer un send aca

				break;

			case 5:

				return EXIT_SUCCESS;
				break;
			case 9:
				listarTablas();
				break;
			case 10:
				imprimirBITARRAY(bitmap);
				break;
			default:

				printf("Operacion desconocida.");
				break;
			}

		retardo();
		free(parametros);

		lineaRequest=ingresar_request();
		while(string_is_empty(lineaRequest)){
			free(lineaRequest);
			lineaRequest = ingresar_request();
		}
		parametros = strtok(lineaRequest, " ");
		string_to_upper(parametros);
		cod_ingresado = codigo_ingresado(parametros);

	}

}


void deployMenu(){
	printf("\n\nCREATE    NOMBRETABLA CONSISTENCIA PARTICIONES TIEMPO_COMPACTACION \nDROP      NOMBRETABLA\nDESCRIBE  NOMBRETABLA (OPCIONAL)\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY \"VALUE\" TIMESTAMP (OPCIONAL) \n");
	printf("\nIngrese REQUEST\n");



}


void imprimirMetadataDeTabla(char* nombre_tabla){
	t_metadata_fs* metadata = obtenerMetadataTabla(nombre_tabla);
	printf("%s\t",nombre_tabla);
	imprimirMetadata(metadata);
	free(metadata);

}


char* ingresar_request()
{

	deployMenu();

	  char * linea;

	  while(1) {
		linea = readline(">");
		if(linea)add_history(linea);

		return linea;
		free(linea);
	  }
}

int codigo_ingresado(char* codOp){

	if (strcmp(codOp, "CREATE")==0) {
		return 0;
	}
	else if (strcmp(codOp, "DROP")==0) {
		return 1;
	}
	else if (strcmp(codOp, "DESCRIBE")==0) {
		return 2;
	}
	else if (strcmp(codOp, "SELECT")==0) {
		return 3;
	}
	else if (strcmp(codOp, "INSERT")==0) {
		return 4;
	}
	else if (strcmp(codOp, "LS")==0) {
		return 9;
	}
	else if (strcmp(codOp, "BA")==0) {
			return 10;
	}
	else if (strcmp(codOp, "EXIT")==0){
		return 5;
	}

	return 6;
}

t_paquete_drop* LeerDrop(char *parametros){
	char* nombre_tabla;
	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		faltaTabla();
		return NULL;
	}
	nombre_tabla = parametros;
	t_paquete_drop* paquete = crear_paquete_drop(nombre_tabla);
	loggear_request_drop(paquete);

	return paquete;

}

t_paquete_describe* LeerDescribe(char *parametros){
	char* nombre_tabla;
	parametros= strtok(NULL, " ");
	if(parametros == NULL){
		loggear_request_describe_sin_tabla();
		return NULL;
	}
	else
	{
		nombre_tabla = parametros;
		t_paquete_describe* paquete = crear_paquete_describe(nombre_tabla);
		loggear_request_describe_con_tabla(paquete);
		return paquete;
	}

}

t_paquete_select* LeerSelect(char* parametros){


	uint16_t valor_key;
	char* nombre_tabla;

	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		faltaTabla();
		return NULL;
	}
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validarNumero(parametros)){
		faltaKey();
		return NULL;
	}
	valor_key = atoi(parametros);

	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);

	loggear_request_select(paquete);

	return paquete;
}

t_paquete_insert* LeerInsert(char* parametros){

	uint16_t valor_key;
	char* nombre_tabla;
	char* value;
	long long timestamp;

	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		faltaTabla();
		return NULL;
	}

	nombre_tabla = parametros;
	string_to_upper(nombre_tabla);

	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validarNumero(parametros)){
		faltaKey();
		return NULL;
	}
	valor_key = atoi(parametros);

	parametros = strtok(NULL, " ");



	char* valueCompleto =string_new();
	if(parametros==NULL){
		faltaEnmascararValue();
		return NULL;
	}
	string_append(&valueCompleto, parametros);
	if(parametros!=NULL && (string_starts_with(parametros, "\"") && !string_ends_with(parametros, "\""))){

		while(!string_ends_with(valueCompleto, "\"")){
			parametros = strtok(NULL, " ");
			if(parametros==NULL){
				faltaValue();
				return NULL;

			}
			string_append(&valueCompleto, " ");
			string_append(&valueCompleto, parametros);

		}


	}else if(parametros==NULL || !(string_starts_with(parametros, "\"")) || !(string_ends_with(parametros, "\""))){
		faltaEnmascararValue();
		return NULL;
	}

	char* valueSinPrimeraComilla = string_substring_from(valueCompleto,1);
	char* valueDadoVuelta =string_reverse(valueSinPrimeraComilla);
	char* valueSinComillaFinal=	string_substring_from(valueDadoVuelta,1);
	char* valueFinal = string_reverse(valueSinComillaFinal);
	value=valueFinal;



	//parametros = strtok(NULL, "\"");

	t_config* config =leer_config();
	int maxValue = config_get_int_value(config,"TAMAÑO_VALUE");
	if(value=="" || string_length(value)>maxValue){
		faltaValue();
		free(valueCompleto);
		free(valueSinPrimeraComilla);
		free(valueDadoVuelta);
		free(valueSinComillaFinal);
		free(valueFinal);
		config_destroy(config);
		return NULL;
	}
	config_destroy(config);



	parametros = strtok(NULL, " ");
	if (parametros==NULL) {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	timestamp = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {
		if(!validarNumero(parametros)){
		faltaTimestamp();
		free(valueCompleto);
		free(valueSinPrimeraComilla);
		free(valueDadoVuelta);
		free(valueSinComillaFinal);
		free(valueFinal);
		return NULL;
		}
		timestamp = atoll(parametros);
	}

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);
	free(valueCompleto);
	free(valueSinPrimeraComilla);
	free(valueDadoVuelta);
	free(valueSinComillaFinal);
	free(valueFinal);
	return paquete;
}

t_paquete_insert* LeerInsert2(char* parametros){

	uint16_t valor_key;
	char* nombre_tabla;
	char* value;
	long long timestamp;

	char** TodosLosParametros= string_n_split(parametros, 4, " ");
	printf("%s\n",TodosLosParametros[1]);
//	printf("%s\n",TodosLosParametros[1]);
//	printf("%s\n",TodosLosParametros[2]);



	if(TodosLosParametros[1]==NULL){
		faltaTabla();
		return NULL;
	}
	nombre_tabla = TodosLosParametros[1];

//	parametros = strtok(NULL, " ");
	if(TodosLosParametros[2]==NULL || !validarNumero(TodosLosParametros[2])){
		faltaKey();
		return NULL;
	}
	valor_key = atoi(TodosLosParametros[2]);

//	parametros = strtok(NULL, "\"");



	char** valueYTime =string_split(TodosLosParametros[3], " ");


	if(valueYTime[0]==NULL || !(string_starts_with(valueYTime[0], "\"") && string_ends_with(valueYTime[0], "\""))){
		faltaEnmascararValue();
		return NULL;
	}
	char* valueSinPrimeraComilla = string_substring_from(valueYTime[0],1);
	char* valueDadoVuelta =string_reverse(valueSinPrimeraComilla);
	char* valueSinComillaFinal=	string_substring_from(valueDadoVuelta,1);
//
	value=valueSinComillaFinal;



	//parametros = strtok(NULL, "\"");

	t_config* config =leer_config();
	int maxValue = config_get_int_value(config,"TAMAÑO_VALUE");
	if(value=="" || string_length(value)>maxValue){
		faltaValue();
//		free(valueSinPrimeraComilla);
//		free(valueDadoVuelta);
//		free(valueSinComillaFinal);
		return NULL;
	}
	config_destroy(config);



//	parametros = strtok(NULL, " ");
//	printf("%s",parametros);

	if (valueYTime[1]==NULL) {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	timestamp = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {
		if(!validarNumero(valueYTime[1])){
		faltaTimestamp();
//		free(valueSinPrimeraComilla);
//		free(valueDadoVuelta);
//		free(valueSinComillaFinal);
		return NULL;
		}
		timestamp = atoll(valueYTime[1]);
	}

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);
//	free(valueSinPrimeraComilla);
//	free(valueDadoVuelta);
//	free(valueSinComillaFinal);

	return paquete;
}


t_paquete_create* LeerCreate(char* parametros){



	char* nombre_tabla;
	consistency consistencia;
	int particiones;
	int compactacion;

	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		faltaTabla();
		return NULL;
	}
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	if(parametros != NULL) string_to_upper(parametros);
	if(parametros==NULL || !validarConsistencia(parametros)){
		faltaConsistencia();
		return NULL;
	}

	consistencia = pasarAConsistenciaINT(parametros);
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validarNumero(parametros) || !strcmp(parametros,"0")){
		faltaParticiones();
		return NULL;
	}
	particiones = atoi(parametros);
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validarNumero(parametros)){
		faltaTiempo_Compactacion();
		return NULL;
	}
	compactacion = atoi(parametros);

	t_paquete_create* paquete = crear_paquete_create(nombre_tabla, consistencia,particiones,compactacion);


	loggear_request_create_fs(paquete);

	return paquete;
}

bool validarNumero(char* parametro){
	for(int i=0;i<string_length(parametro);i++){
		if(!isdigit(parametro[i])) return false;
	}
	return true;
}


void loggear_request_select(t_paquete_select* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: SELECT %s %d\n", paquete->nombre_tabla,paquete->valor_key);
    log_destroy(logger);
}
void loggear_request_insert(t_paquete_insert* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: INSERT %s %d %s %lli\n", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
    log_destroy(logger);
}
void loggear_request_create_mp(t_paquete_create_de_mp* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: CREATE %s %s %d %d \n", paquete->nombre_tabla,paquete->consistencia,paquete->particiones,paquete->tiempo_compactacion);
    log_destroy(logger);
}

void loggear_request_create_fs(t_paquete_create* paquete_create_fs){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: CREATE %s %s %d %d \n", paquete_create_fs->nombre_tabla,pasarAConsistenciaChar(paquete_create_fs->metadata.consistencia),paquete_create_fs->metadata.particiones,paquete_create_fs->metadata.tiempo_de_compactacion);
    log_destroy(logger);
}

void loggear_request_drop(t_paquete_drop* paquete)
{
	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: DROP %s\n",paquete->nombre_tabla);
	log_destroy(logger);
}

void loggear_request_describe_con_tabla(t_paquete_describe* paquete){
	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: DESCRIBE DE %s\n",paquete->nombre_tabla);
	log_destroy(logger);
}

void loggear_request_describe_sin_tabla(){
	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: DESCRIBE DE TODAS LAS TABLAS\n");
	log_destroy(logger);
}

t_paquete_select* crear_paquete_select(char* nombretabla, uint16_t valor_key) //Agregado
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_select* paquete = malloc(sizeof(t_paquete_select));
	paquete->nombre_tabla=malloc(tamaniotabla);
	strcpy(paquete->nombre_tabla, nombretabla);
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamaniotabla;

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

t_paquete_describe* crear_paquete_describe(char *nombre_tabla)
{
	uint32_t tamanio_tabla= strlen(nombre_tabla)+1;
	t_paquete_describe* paquete= malloc(tamanio_tabla + sizeof(int)+4);

	paquete->nombre_tabla = nombre_tabla;
	paquete->nombre_tabla_long = tamanio_tabla;

	return paquete;
}

t_paquete_insert* crear_paquete_insert(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp) //Agregado
{


	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_value = strlen(value)+1;
	t_paquete_insert* paquete = malloc(sizeof(t_paquete_insert));//malloc(sizeof(int)*2 + tamanio_tabla + tamanio_value + sizeof(uint16_t)+sizeof(long long));
	paquete->nombre_tabla=malloc(tamanio_tabla);
	paquete->value=malloc(tamanio_value);


	strcpy(paquete->nombre_tabla,nombre_tabla);
	strcpy(paquete->value , value);
	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;
	paquete->nombre_tabla_long= tamanio_tabla;
	paquete->value_long= tamanio_value;

	return paquete;
}

t_paquete_create* crear_paquete_create(char* nombretabla, consistency consistencia, int particiones,int tiempo_de_compactacion) //Agregado
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_create* paquete = malloc(sizeof(t_paquete_create));
	paquete->nombre_tabla=malloc(tamaniotabla);

	strcpy(paquete->nombre_tabla,nombretabla);
	paquete->metadata.consistencia = consistencia;
	paquete->metadata.particiones = particiones;
	paquete->metadata.tiempo_de_compactacion= tiempo_de_compactacion;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;

}

void retardo(){
	t_config* config =leer_config();
	int retardo = config_get_int_value(config,"RETARDO");
//	struct timespec tim,tim2;
//	tim.tv_sec = 0;
//	tim.tv_nsec = retardo*1000000;
//	nanosleep(&tim,&tim2);
	int retardoEnMicroSegundos= retardo*1000;
	usleep(retardoEnMicroSegundos);
	config_destroy(config);
}




