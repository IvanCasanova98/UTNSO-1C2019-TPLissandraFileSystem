/*
 * Consola.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include"Consola.h"

void* prenderConsola(void* arg){


	char* lineaRequest=NULL;

	while(lineaRequest==NULL){
		lineaRequest = ingresar_request();
	}

	char* parametros = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros);
	while(1)
	{
		switch(cod_ingresado){
			case 0:;
				t_paquete_create* paquete_create =LeerCreate(parametros) ;
				APIcreate(paquete_create);
				free(paquete_create);
				break;
			case 3:;

				t_paquete_select* paquete_select =LeerSelect(parametros) ;
				free(paquete_select);

				break;

			case 4:;

				t_paquete_insert* paquete_insert=LeerInsert(parametros);
				free(paquete_insert);

				break;

			case 5:

				return EXIT_SUCCESS;
			default:

				printf("Operacion desconocida.");
				break;
			}

		free(parametros);
		lineaRequest = ingresar_request();
		parametros = strtok(lineaRequest, " ");
		cod_ingresado = codigo_ingresado(parametros);

	}

}
void deployMenu(){
	printf("\n\nCREATE NOMBRETABLA CONSISTENCIA PARTICIONES TIEMPO_COMPACTACION \nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY VALUE TIMESTAMP (OPCIONAL) \n");
	printf("\nIngrese REQUEST ");
	nodoTablaMemTable* nodo1 = crearNodoTabla("Tabla1");
	nodoTablaMemTable* nodo2 = crearNodoTabla("Tabla2");
	nodoTablaMemTable* nodo3 = crearNodoTabla("Tabla3");
	nodoRegistroMemTable* registro1 = crearNodoRegistro("Registro1",2,10);
	nodoRegistroMemTable* registro2 = crearNodoRegistro("Registro2",2000,10000);
	agregarTabla(nodo1);
	agregarTabla(nodo2);
	agregarTabla(nodo3);
	agregarRegistro(nodo2, registro1);
	agregarRegistro(nodo2, registro2);
	imprimirRegistrosTabla(nodo2);


	eliminarNodoTabla(nodo1);
	eliminarNodoTabla(nodo2);
	eliminarNodoTabla(nodo3);
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

	return EXIT_FAILURE;
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
	parametros = strtok(NULL, " ");
	value = parametros;
	parametros = strtok(NULL, " ");

	if (parametros==NULL) {
	struct timeval te;
	gettimeofday(&te, NULL); // get current time
	timestamp = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {timestamp = atoi(parametros);}

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);
	loggear_request_insert(paquete);

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
	log_info(logger, "NUEVA REQUEST: INSERT %s %d %s %d\n", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
    log_destroy(logger);
}
void loggear_request_create(t_paquete_create* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST: CREATE %s %s %d %d \n", paquete->nombre_tabla,pasarAConsistenciaChar(paquete->metadata.consistencia),paquete->metadata.particiones,paquete->metadata.tiempo_de_compactacion);
    log_destroy(logger);
}


t_paquete_select* crear_paquete_select(char* nombretabla, uint16_t valor_key) //Agregado
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_select* paquete = malloc(sizeof(long long)+sizeof(uint16_t)+tamaniotabla);

	paquete->nombre_tabla= nombretabla;
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;

}

t_paquete_insert* crear_paquete_insert(char *nombre_tabla, uint16_t valor_key, char *value, int timestamp) //Agregado
{


	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_value = strlen(value)+1;
	t_paquete_insert* paquete = malloc(sizeof(int)*2 + tamanio_tabla + tamanio_value + sizeof(uint16_t)+sizeof(long long));

	paquete->nombre_tabla= nombre_tabla;
	paquete->value = value;
	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;
	paquete->nombre_tabla_long= tamanio_tabla;
	paquete->value_long= tamanio_value;

	return paquete;
}

t_paquete_create* crear_paquete_create(char* nombretabla, consistency consistencia, int particiones,int tiempo_de_compactacion) //Agregado
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_create* paquete = malloc(sizeof(metadata)+tamaniotabla);

	paquete->nombre_tabla= nombretabla;
	paquete->metadata.consistencia = consistencia;
	paquete->metadata.particiones = particiones;
	paquete->metadata.tiempo_de_compactacion = tiempo_de_compactacion;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;

}






