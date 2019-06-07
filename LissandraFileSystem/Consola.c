/*
 * Consola.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include"Consola.h"


void* prenderConsola(void* arg){


	char *lineaRequest = ingresar_request();

	while(string_is_empty(lineaRequest )){
		free(lineaRequest);
		lineaRequest = ingresar_request();

	}

	char* parametros = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros);
	//free(lineaRequest);
	while(1)
	{
		switch(cod_ingresado){
			case 0:;
				t_paquete_create* paquete_create =LeerCreate(parametros) ;
				APIcreate(paquete_create);
				break;
			case 1:;
				t_paquete_drop* paquete_drop =LeerDrop(parametros);
				APIdrop(paquete_drop);
				free(paquete_drop);
				break;
			case 2:;
				t_paquete_describe* paquete_describe = LeerDescribe(parametros);
				if(paquete_describe == NULL){
					APIdescribeTodasLasTablas();
				} else{
					t_metadata* metadataDeTablaPedida = APIdescribe(paquete_describe);
					if(metadataDeTablaPedida != NULL) imprimirMetadata(metadataDeTablaPedida);
					free(metadataDeTablaPedida);
				}
				free(paquete_describe);
				break;
			case 3:;

				t_paquete_select* paquete_select =LeerSelect(parametros) ;
				void* value=APIselect(paquete_select);
				if(value!=NULL){
					//tendria que hacer el send aca
					free(value);
				}

				break;
			case 4:;

				t_paquete_insert* paquete_insert=LeerInsert(parametros);
				APIinsert(paquete_insert);
				//en recibir.h tendria que hacer un send aca

				break;

			case 5:

				return EXIT_SUCCESS;
				break;
			default:

				printf("Operacion desconocida.");
				break;
			}
		//free(lineaRequest);
		free(parametros);
		chekearDumpeo();
		//free(lineaRequest);
		lineaRequest=ingresar_request();
		while(string_is_empty(lineaRequest)){
			free(lineaRequest);
			lineaRequest = ingresar_request();
		}
		parametros = strtok(lineaRequest, " ");
		cod_ingresado = codigo_ingresado(parametros);

	}

}

void imprimirMetadataDeTabla(char* nombre_tabla){
	t_metadata* metadata = obtenerMetadataTabla(nombre_tabla);
	imprimirMetadata(metadata);
	free(metadata);

}

void deployMenu(){
	printf("\n\nCREATE NOMBRETABLA CONSISTENCIA PARTICIONES TIEMPO_COMPACTACION \nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY \"VALUE\" TIMESTAMP (OPCIONAL) \nDESCRIBE NOMBRETABLA\nDROP\t NOMBRETABLA\n");
	printf("\nIngrese REQUEST\n");

	listarTablas();


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

	}if (strcmp(codOp, "EXIT")==0)
	{return 5;}

	return 6;
}

t_paquete_drop* LeerDrop(char *parametros){
	char* nombre_tabla;
	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;

	t_paquete_drop* paquete = crear_paquete_drop(nombre_tabla);
	loggear_request_drop(paquete);

	return paquete;

}

t_paquete_describe* LeerDescribe(char *parametros){
	char* nombre_tabla;
	parametros= strtok(NULL, " ");
	if(parametros == NULL){
		printf("DESCRIBE SOLO\n");
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
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
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
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	valor_key = atoi(parametros);
	parametros = strtok(NULL, "\"");
	value=parametros;
	parametros = strtok(NULL, " ");

	if (parametros==NULL) {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	timestamp = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {timestamp = atoll(parametros);}

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);


	return paquete;
}

t_paquete_create* LeerCreate(char* parametros){



	char* nombre_tabla;
	consistency consistencia;
	int particiones;
	int compactacion;

	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	consistencia = pasarAConsistenciaINT(parametros);
	parametros = strtok(NULL, " ");
	particiones = atoi(parametros);
	parametros = strtok(NULL, " ");
	compactacion = atoi(parametros);

	t_paquete_create* paquete = crear_paquete_create(nombre_tabla, consistencia,particiones,compactacion);

	loggear_request_create(paquete);

	return paquete;
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
void loggear_request_create(t_paquete_create* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: CREATE %s %s %d %d \n", paquete->nombre_tabla,pasarAConsistenciaChar(paquete->metadata.consistencia),paquete->metadata.particiones,paquete->metadata.tiempo_de_compactacion);
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
	t_paquete_describe* paquete= malloc(tamanio_tabla + sizeof(int));

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
	paquete->metadata.tiempo_de_compactacion = tiempo_de_compactacion;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;

}



