#include "consola.h"

//------------------------------INGRESO DE PAQUETES------------------------------------

void ingresar_paquete(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg ;

	char* lineaRequest = ingresar_request();
	string_to_upper(lineaRequest);
	char* parametros_paquete = strtok(lineaRequest, " ");
	int cod_ingresado = codigo_ingresado(parametros_paquete);

	while(1)
	{
		switch(cod_ingresado)
		{
			case 0:;
				t_paquete_create* paquete_c = paquete_create(parametros_paquete, parametro->logger);
				create(0, paquete_c, parametro->config, parametro->logger);

				break;
			case 1:;
				parametros_paquete= strtok(NULL, " ");
				drop(parametros_paquete,parametro->config,parametro->logger);
				break;

			case 2:;
				t_paquete_describe_lfs* paquete_describe_lfs = paquete_describe_para_lfs(parametros_paquete,parametro->logger);
				enviar_describe_lissandra(paquete_describe_lfs,parametro->config,parametro->logger);
				break;
			case 3:;
				t_paquete_select* paquete_s = paquete_select(parametros_paquete, parametro->logger);
				selectf(NULL,paquete_s, parametro->config, parametro->logger);
				break;
			case 4:;
				t_paquete_insert* paquete_i = paquete_insert(parametros_paquete, parametro->logger);
				insert(paquete_i, parametro->config,  parametro->logger, 1);
//				if(paquete_i != NULL)
//				{
//				enviar_insert_lissandra(paquete_i,parametro->config,parametro->logger);
//				}
				break;
			case 5:;
				journal(parametro->config,parametro->logger);
				//FIJARSE SI LOGEAAA
				break;
			default:
			//	mostrar_tabla_segmentos();
				printf("Operacion desconocida.");
				break;
			}

		lineaRequest = ingresar_request();
		parametros_paquete = strtok(lineaRequest, " ");
		cod_ingresado = codigo_ingresado(parametros_paquete);
	}
}

char* ingresar_request()
{
	printf("\n\nCREATE    NOMBRETABLA CONSISTENCIA PARTICIONES T_COMPACTACION\nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY VALUE \nJOURNAL\nEXIT\n");

	printf("\nIngrese REQUEST ");

	char * linea;

	while(1) {
	linea = readline(">");
	if(linea)add_history(linea);
	return linea;
	free(linea);
	}
}

int codigo_ingresado(char* parametros)
{

	if (strcmp(parametros, "CREATE")==0)
	{
		return 0;
	}
	else if (strcmp(parametros, "DROP")==0)
	{
		return 1;
	}
	else if (strcmp(parametros, "DESCRIBE")==0)
	{
		return 2;
	}
	else if (strcmp(parametros, "SELECT")==0)
	{
		return 3;
	}
	else if (strcmp(parametros, "INSERT")==0)
	{
		return 4;
	}
	else if (strcmp(parametros, "JOURNAL")==0)
	{
		return 5;
	}
	else {return 6;}
}


