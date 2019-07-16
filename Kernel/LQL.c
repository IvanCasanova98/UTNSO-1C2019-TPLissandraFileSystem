#include "LQL.h"

t_queue* lql_to_queue(char* path)
{
	t_queue* proceso_lql = queue_create();

	FILE *archivo_LQL = fopen(path,"r");
	char* lineaRequest[80];
	while(!feof(archivo_LQL))
	{
		fscanf(archivo_LQL," %[^\n]",&lineaRequest);

		char* linea_request = malloc(strlen(lineaRequest)+1);
		strcpy(linea_request, lineaRequest);

		queue_push(proceso_lql,linea_request);
	}
	fclose(archivo_LQL);
	return proceso_lql;
}


int tamanio_lql(t_queue* proceso_lql)
{
	int tamanio = 0;
	t_list* lista_lql = list_create();
	lista_lql = proceso_lql->elements;

	void _aumentar_tamanio(char* elemento){tamanio += strlen(elemento);}

	list_iterate(lista_lql, _aumentar_tamanio);


	return tamanio;
}

//void funcion_LQL(char* path, int conexion, t_config* config){
//	path= strtok(NULL, " ");
//	string_to_lower(path);
//	path = strcat(path, ".lql");
//	FILE *archivo_LQL = fopen(path,"r");
//	char* lineaRequest[80];
//	while(!feof(archivo_LQL)){
//		fscanf(archivo_LQL," %[^\n]",&lineaRequest);
//		seleccionar_paquete(lineaRequest, conexion, config);
//
//	}
//}
