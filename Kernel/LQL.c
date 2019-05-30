#include "LQL.h"

void funcion_LQL(char* path, int conexion){
	path= strtok(NULL, " ");

	FILE *archivo_LQL = fopen(path,"r");
	char* lineaRequest[80];
	while(!feof(archivo_LQL)){
		fscanf(archivo_LQL," %[^\n]",&lineaRequest);
		seleccionar_paquete(lineaRequest, conexion);
		//printf("%s\n",lineaRequest);
	}
}

void seleccionar_paquete(char* lineaRequest, int conexion){

	char* parametros = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros);

	switch(cod_ingresado){

		case 0:;

			t_paquete_create* paquete_create = create(parametros);

			if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
			else
			{
			}

			free(paquete_create);

			break;

		case 3:;

			t_paquete_select* paquete_select = selectf(parametros);

			if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
			else
			{
				enviar_paquete_select(paquete_select, conexion);
				recibir_mensaje(conexion);
			}

			free(paquete_select);
			break;

		case 4:;

			t_paquete_insert* paquete_insert = insert(parametros);

			if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
			else{enviar_paquete_insert(paquete_insert, conexion);}
			free(paquete_insert);

			break;

		case 8:;
			funcion_LQL(parametros, conexion);

			break;

		case 9:
			return;
		default:

			printf("Operacion desconocida.");
			break;
	}

}
