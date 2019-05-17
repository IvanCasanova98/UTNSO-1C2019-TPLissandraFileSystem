#include "consola.h"

//------------------------------INGRESO DE PAQUETES------------------------------------

void ingresar_paquete(){

	char* lineaRequest = ingresar_request();
	char* parametros = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros);

	while(1)
	{
		switch(cod_ingresado){

			case 3:;

				t_log* logger_s = iniciar_logger();
				t_config* config_s = leer_config();

				int conexion_s = iniciar_conexion(logger_s, config_s);

				t_paquete_select* paquete_s = paquete_select(parametros);

				if (send(conexion_s, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else{enviar_paquete_select(paquete_s, conexion_s);}
				free(paquete_s);

				terminar_conexion(logger_s,config_s,conexion_s);

				break;

			case 4:;

				t_log* logger_i = iniciar_logger();
				t_config* config_i = leer_config();

				int conexion_i = iniciar_conexion(logger_i, config_i);

				t_paquete_insert* paquete_i = paquete_insert(parametros);

				if (send(conexion_i, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else{enviar_paquete_insert(paquete_i, conexion_i);}
				free(paquete_i);

				terminar_conexion(logger_i,config_i,conexion_i);

				break;

			case 8:
				return;
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

int codigo_ingresado(char* parametros){

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
	else {return 8;}
}


