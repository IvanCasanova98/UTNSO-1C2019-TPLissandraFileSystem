#include "consola.h"
void consola()
{
	while(1)
	{
		char* lineaRequest = ingresar_request();
		string_to_upper(lineaRequest);
		agregar_a_cola(lineaRequest);
	}
}

//------------------------------INGRESO DE PAQUETES------------------------------------
void request(void * arg) //RECIBIR LOGGER
{

	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	char** vector_request = string_split(parametro->parametros," ");

	int cod_ingresado = codigo_ingresado(vector_request[0]);

	retardo_ejecucion(parametro->config);
		switch(cod_ingresado){
			case 0:;


				t_paquete_create* paquete_create = create(vector_request);

				if(paquete_create==NULL){break;}

				if (send(parametro->conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else
				{
					enviar_paquete_create(paquete_create,parametro->conexion);
				}

				char* nombre_tabla = paquete_create->nombre_tabla;

				describe(parametro->conexion,nombre_tabla);

				free(paquete_create);

				break;

			case 2:;
				describe(parametro->conexion,vector_request[1]);
				break;

			case 3:;
				t_paquete_select* paquete_select = selectf(vector_request);

				if(paquete_select==NULL){break;}

				if (send(parametro->conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else
				{
					enviar_paquete_select(paquete_select, parametro->conexion);
					recibir_mensaje(parametro->conexion);
				}

				free(paquete_select);
				break;

			case 4:;

				t_paquete_insert* paquete_insert = insert(vector_request);

				if(paquete_insert==NULL){break;}

				if(existe_tabla(paquete_insert->nombre_tabla)) //HACER DESCRIBE CON CREATE
				{
					if (send(parametro->conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
					else{enviar_paquete_insert(paquete_insert, parametro->conexion);}
				}
				else
				{
					//pedir describe
					t_log* logger = iniciar_logger();
					log_error(logger, "Error: no existe la tabla\n");
					log_destroy(logger);
				}
				free(paquete_insert);
				break;
			default:
				printf("\nCAIGO ACA PADRE, TE ESTAS EQUIVOCANDO");
				break;
		}

}

char* ingresar_request()
{
	printf("\n\nCREATE    NOMBRETABLA CONSISTENCIA PARTICIONES T_COMPACTACION\nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY VALUE \nJOURNAL\nRUN    PATH\nADD\nEXIT\n");
	printf("\nIngrese REQUEST ");
	char * linea;

	while(1)
	{
		linea = readline(">");
		if(linea)add_history(linea);
		return linea;
		free(linea);
	}
}
