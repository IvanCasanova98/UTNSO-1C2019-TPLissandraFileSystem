#include "LQL.h"

void funcion_LQL(char* path, int conexion){
	path= strtok(NULL, " ");

	FILE *archivo_LQL = fopen(path,"r");
	char* lineaRequest[80];
	while(!feof(archivo_LQL)){
		fscanf(archivo_LQL," %[^\n]",&lineaRequest);

		seleccionar_paquete(lineaRequest, conexion);

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
				enviar_paquete_create(paquete_create,conexion);
			}

			free(paquete_create);

			break;

		case 2:;
			parametros= strtok(NULL, " ");
			send(conexion, &cod_ingresado, sizeof(int), 0);

			if (parametros==NULL)
			{
				int tamanio = strlen("ALL")+1;
				char* buffer = "ALL";
				send(conexion, &tamanio, sizeof(int), 0);
				send(conexion, buffer, tamanio, 0);
				logger_describe_all();
			}
			else
			{
				int tamanio = strlen(parametros)+1;
				send(conexion, &tamanio, sizeof(int), 0);
				send(conexion, parametros, tamanio, 0);
				logger_describe_tabla(parametros);
			}
			deserealizar_metadata(conexion);
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
		case 9:
			funcion_LQL(parametros, conexion);
			break;
		default:
			printf("Operacion desconocida.\n\n");
			break;
	}
}
