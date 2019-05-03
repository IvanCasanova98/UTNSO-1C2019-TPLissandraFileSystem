#include "paquete.h"


t_paquete_select* crear_paquete_select(char* nombretabla,int valor_key) //Agregado
{
	uint32_t tamaniotabla = strlen(nombretabla)+1;
	t_paquete_select* paquete = malloc(8+tamaniotabla);
	paquete->nombre_tabla= nombretabla;
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamaniotabla;
	return paquete;

}

t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp) //Agregado
{
	t_paquete_insert* paquete = malloc(35);

	for(int i=0;i<7;i++){
		paquete->nombre_tabla[i]= nombretabla[i];
	}
	for(int i=0;i<20;i++){
		paquete->value[i] = value[i];
	}
	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;


	return paquete;
}

//---------------------ARMAR PAQUETES

t_paquete_select* selectf(){


	int valor_key;
	char* nombre_tabla;

	printf("\nIngrese nombre de la tabla: ");
	scanf("%s", nombre_tabla);
	printf("\nIngrese el valor de la key: ");
	scanf("%d", &valor_key);

	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);
	loggear_paquete_select(paquete);

	return paquete;
}

t_paquete_insert* insert(){

	int valor_key;
	char nombre_tabla[7];
	char value[20];
	int timestamp;

	printf("\nIngrese nombre de la tabla: ");
	scanf("%s", &nombre_tabla);
	printf("\nIngrese el valor de la key: ");
	scanf("%d", &valor_key);
	printf("\nIngrese el value: ");
	scanf("%s", &value);
	printf("\nIngrese el timestamp: ");
	scanf("%d", &timestamp);

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);
	loggear_paquete_insert(paquete);

	return paquete;
}

//----------------------------------------------------LOGGEO DE PAQUETES

void loggear_paquete_select(t_paquete_select* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "\nNuevo paquete creado\nTipo paquete: SELECT\nNombre tabla: %s\nValor KEY: %d", paquete->nombre_tabla,paquete->valor_key);
    log_destroy(logger);
}

void loggear_paquete_insert(t_paquete_insert* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "\nNuevo paquete creado\nTipo paquete: INSERT\nNombre tabla: %s\nValor KEY: %d\nValue del paquete: %s\nTimestamp: %d", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
    log_destroy(logger);
}

//----------------------------------------------------ELIMINAR PAQUETE

//void eliminar_paquete_select(t_paquete_select* paquete)
//{
//    free(paquete->buffer->stream);
//    free(paquete->buffer);
//    free(paquete);
//}

//void eliminar_paquete_insert(t_paquete_insert* paquete)
//{
//    free(paquete->buffer->stream);
//    free(paquete->buffer);
//    free(paquete);
//}
