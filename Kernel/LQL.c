#include "LQL.h"

void funcion_LQL(char* path, int conexion, t_config* config){
	path= strtok(NULL, " ");
	string_to_lower(path);
	path = strcat(path, ".lql");
	FILE *archivo_LQL = fopen(path,"r");
	char* lineaRequest[80];
	while(!feof(archivo_LQL)){
		fscanf(archivo_LQL," %[^\n]",&lineaRequest);

		seleccionar_paquete(lineaRequest, conexion, config);

	}
}

void seleccionar_paquete(char* lineaRequest, int conexion, t_config* config){

	char* parametros = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros);

	switch(cod_ingresado){
		retardo_ejecucion(config);
		case 0:;

			t_paquete_create* paquete_create = create(parametros);

			if(paquete_create==NULL){break;}

			if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
			else
			{
				enviar_paquete_create(paquete_create,conexion);
			}



			char* nombre_tabla = paquete_create->nombre_tabla;
			describe(conexion,nombre_tabla);



			free(paquete_create);

			break;

		case 2:;

			parametros= strtok(NULL, " ");
			describe(conexion,parametros);
			break;
		case 3:;

			t_paquete_select* paquete_select = selectf(parametros);

			if(paquete_select==NULL){break;}

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

			if(paquete_insert==NULL){break;}

			if(existe_tabla(paquete_insert->nombre_tabla)) //HACER DESCRIBE CON CREATE
			{
				if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else{enviar_paquete_insert(paquete_insert, conexion);}
				free(paquete_insert);
			}
			else
			{
				t_log* logger = iniciar_logger();
				log_error(logger, "Error: no existe la tabla\n");
				log_destroy(logger);
			}


			break;
		case 6:
			funcion_LQL(parametros, conexion,config);
			break;
		default:
			printf("Operacion desconocida.\n\n");
			break;
		}
}
