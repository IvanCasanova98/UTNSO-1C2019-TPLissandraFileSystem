#include "kernel.h"

int main(void){

	t_log* logger = iniciar_logger();

	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);

	armar_paquete(conexion);

	terminar_conexion(logger,config,conexion);


}

void armar_paquete(int conexion){

	int cod_ingresado;

	system("clear");
	printf("\nCREATE\nDROP\nDESCRIBE\nSELECT NOMBRETABLA KEY\nINSERT NOMBRETABLA KEY VALUE TIMESTAMP\nJOURNAL\nRUN\nADD\n");
	printf("\nIngrese REQUEST ");
	char* lineaRequest =requestIngresada();
	char *parametros = strtok(lineaRequest, " "); //strok separa los parametros ej: "SELECT" "KEY" "TAVLA"
	cod_ingresado=convertirStringEnOpCode(parametros);

	while(1)

	{
		switch(cod_ingresado){

			//***************************** SELECT *****************************
			case 3:;
			//******************************************************************

				t_paquete_select* paquete_select = selectf(parametros);

				if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else {
					t_log* logger = iniciar_logger();
					log_info(logger, "Se envio codigo de operacion: %d",cod_ingresado);
					log_destroy(logger);
				}

				enviar_paquete_select(paquete_select, conexion);
				free(paquete_select);
				break;

			//***************************** INSERT *****************************
			case 4:;
			//******************************************************************
				t_paquete_insert* paquete_insert = insert(parametros);

				if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else {
					t_log* logger = iniciar_logger();
					log_info(logger, "Se envio codigo de operacion: %d",cod_ingresado);
					log_destroy(logger);
				}
				enviar_paquete_insert(paquete_insert, conexion);
				free(paquete_insert);

				break;
			//******************************************************************
			default:
			//******************************************************************
				printf("Operacion desconocida.");
				break;
			}


		system("clear");
		printf("\nCREATE\nDROP\nDESCRIBE\nSELECT NOMBRETABLA KEY\nINSERT NOMBRETABLA KEY VALUE TIMESTAMP\nJOURNAL\nRUN\nADD\n");
		printf("\nIngrese REQUEST ");
		lineaRequest =requestIngresada();
		parametros = strtok(lineaRequest, " "); //strok separa los parametros ej: "SELECT" "KEY" "TAVLA"
		cod_ingresado=convertirStringEnOpCode(parametros);

	}


	system("clear");
	t_log* logger = iniciar_logger();
	log_info(logger, "Kernel Desconectado");
	log_destroy(logger);

  }

char* requestIngresada()
{
	  char * linea;
	  while(1) {
	    linea = readline(":");

	    if(linea)
	      add_history(linea);
	    if(!strncmp(linea, "exit", 4)) {
	       free(linea);
	       return NULL;
	       break;
	    }
	    return linea;
	    free(linea);
	  }


}

int convertirStringEnOpCode(char* string){

	if (strcmp(string, "CREATE")==0) {
		return 0;
	}
	if (strcmp(string, "DROP")==0) {
		return 1;
	}
	if (strcmp(string, "DESCRIBE")==0) {
		return 2;
	}
	if (strcmp(string, "SELECT")==0) {
		return 3;
	}
	if (strcmp(string, "INSERT")==0) {
		return 4;
	}
	if (strcmp(string, "JOURNAL")==0) {
		return 5;
	}
	if (strcmp(string, "RUN")==0) {
		return 6;
	}
	if (strcmp(string, "ADD")==0) {
		return 7;
	}

}




