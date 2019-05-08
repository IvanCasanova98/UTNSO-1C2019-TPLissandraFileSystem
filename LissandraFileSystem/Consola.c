/*
 * Consola.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include"Consola.h"

void* prenderConsola(void* arg){


	//t_log* logger= (t_log *)arg;
	char* lineaRequest = ingresar_request();
	char* parametros = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros);
	while(1)
	{
		switch(cod_ingresado){

			case 3:;

				t_paquete_select* paquete_select =selectf(parametros) ;


				free(paquete_select);

				break;

			case 4:;

				t_paquete_insert* paquete_insert=insert(parametros);


				free(paquete_insert);

				break;

			case 8:
				return -1;
			default:

				printf("Operacion desconocida.");
				break;
			}

		lineaRequest = ingresar_request();
		parametros = strtok(lineaRequest, " ");
		cod_ingresado = codigo_ingresado(parametros);

	}

}

char* ingresar_request()
{
	printf("\n\nCREATE\nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY VALUE \nJOURNAL\nRUN\nADD\nEXIT\n");

	printf("\nIngrese REQUEST ");

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
	else if (strcmp(codOp, "JOURNAL")==0) {
		return 5;
	}
	else if (strcmp(codOp, "RUN")==0) {
		return 6;
	}
	else if (strcmp(codOp, "ADD")==0) {
		return 7;
	}
	else {return 8;}
}
t_paquete_select* selectf(char* parametros){


	uint16_t valor_key;
	char* nombre_tabla;

	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	valor_key = atoi(parametros);
	printf("%d",valor_key);
	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);

	loggear_request_select(paquete);

	return paquete;
}

t_paquete_insert* insert(char* parametros){

	uint16_t valor_key;
	char* nombre_tabla;
	char* value;
	int timestamp=0;

	parametros= strtok(NULL, " ");
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	valor_key = atoi(parametros);
	parametros = strtok(NULL, " ");
	value = parametros;
	parametros = strtok(NULL, " ");
	timestamp = atoi(parametros);
	printf("%d",valor_key);
	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);

	loggear_request_insert(paquete);

	return paquete;
}



void loggear_request_select(t_paquete_select* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST SELECT \nNombre tabla: %s\nValor KEY   : %d", paquete->nombre_tabla,paquete->valor_key);
    log_destroy(logger);
}

void loggear_request_insert(t_paquete_insert* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "NUEVA REQUEST INSERT CREADO\nNombre tabla: %s\nValor KEY   : %d\nValue       : %s\n", paquete->nombre_tabla, paquete->valor_key, paquete->value);
    log_destroy(logger);
}


t_paquete_select* crear_paquete_select(char* nombretabla, uint16_t valor_key) //Agregado
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_select* paquete = malloc(sizeof(int)+sizeof(uint16_t)+tamaniotabla);

	paquete->nombre_tabla= nombretabla;
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamaniotabla;

	return paquete;

}

t_paquete_insert* crear_paquete_insert(char *nombre_tabla, uint16_t valor_key, char *value, int timestamp) //Agregado
{


	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_value = strlen(value)+1;
	t_paquete_insert* paquete = malloc(sizeof(int)*3 + tamanio_tabla + tamanio_value + sizeof(uint16_t));

	paquete->nombre_tabla= nombre_tabla;
	paquete->value = value;
	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;
	paquete->nombre_tabla_long= tamanio_tabla;
	paquete->value_long= tamanio_value;

	return paquete;
}

