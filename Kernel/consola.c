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

	int conexion_nueva;
	if(cod_ingresado!=7) //SI ES UN ADD NO NECESITO CONEXION
	{
		conexion_nueva = conectarse_a_memoria(vector_request, parametro->logger); //ELIGE UNA MEMORIA, SEGUN EL CRITERIO BASADO EN LA TABLA
	}

	retardo_ejecucion(parametro->config);
		switch(cod_ingresado){
		sleep(1000);
			case 0:;


				t_paquete_create* paquete_create = create(vector_request);

				if(paquete_create==NULL){break;}

				if (send(conexion_nueva, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else
				{
					enviar_paquete_create(paquete_create,conexion_nueva);
				}

				char* nombre_tabla = paquete_create->nombre_tabla;

				describe(conexion_nueva,nombre_tabla);

				free(paquete_create);

				break;

			case 2:;
				describe(conexion_nueva,vector_request[1]);
				break;

			case 3:;
				t_paquete_select* paquete_select = selectf(vector_request);

				if(paquete_select==NULL){break;}

				if (send(conexion_nueva, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else
				{
					enviar_paquete_select(paquete_select, conexion_nueva);
					recibir_mensaje(conexion_nueva);
				}

				free(paquete_select);
				break;

			case 4:;

				t_paquete_insert* paquete_insert = insert(vector_request);

				if(paquete_insert==NULL){break;}

				if(existe_tabla(paquete_insert->nombre_tabla)) //HACER DESCRIBE CON CREATE
				{
					if (send(conexion_nueva, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
					else{enviar_paquete_insert(paquete_insert, conexion_nueva);}
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
			case 7:
				agregar_consistencia(atoi(vector_request[2]),vector_request[4]);
				break;
			default:
				break;
			break;
		}


		//----------------------------------------------------------------------------------------
		//esto no va para mi, pero bueno era para probar todo
		close(conexion_nueva);
		//----------------------------------------------------------------------------------------
}

char* ingresar_request()
{
	printf("\n\nCREATE    NOMBRETABLA CONSISTENCIA PARTICIONES T_COMPACTACION\nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY VALUE \nJOURNAL\nRUN    PATH\nADD MEMORY [#MEMORIA] TO [CONSISTENCY]\nEXIT\n");
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
