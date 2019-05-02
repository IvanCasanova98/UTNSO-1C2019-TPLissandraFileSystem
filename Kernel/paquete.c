#include "paquete.h"

//------------------------CREAR BUFFER
//EL BUFFER LO INICIA VACIO. RESERVA UN ESPACIO MAS

//void crear_buffer_select(t_paquete_select* paquete)
//{
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->size = 0;
//	paquete->buffer->stream = NULL;
//}
//
//void crear_buffer_insert(t_paquete_insert* paquete)
//{
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->size = 0;
//	paquete->buffer->stream = NULL;
//}

//----------------------------------------------------CREACION DE PAQUETES

t_paquete_select* crear_paquete_select(char* nombretabla,int valor_key) //Agregado
{
	t_paquete_select* paquete = malloc(11);
	for(int i=0;i<TAMANIO_NOMBRE_TABLA;i++){
		paquete->nombre_tabla[i]= nombretabla[i];
	}
	paquete->valor_key = valor_key;

	return paquete;

}

//t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp) //Agregado
//{
//	t_paquete_insert* paquete = malloc(sizeof(t_paquete_insert));
//
//	paquete->codigo_operacion = INSERT;
//	for(int i=0;i<20;i++){
//		paquete->nombre_tabla[i] = nombretabla[i];
//		paquete->value[i] = value[i];
//	}
//	paquete->valor_key = valor_key;
//	paquete->timestamp = timestamp;
////	crear_buffer_insert(paquete);
//
//	return paquete;
//}

//---------------------ARMAR PAQUETES

t_paquete_select* selectf(){


	int valor_key;
	char nombre_tabla[TAMANIO_NOMBRE_TABLA];

	printf("\nIngrese nombre de la tabla: ");
	scanf("%s", &nombre_tabla);

	printf("\nIngrese el valor de la key: ");
	scanf("%d", &valor_key);

	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);
	printf("ola");
	loggear_paquete_select(paquete);

	return paquete;
}

//t_paquete_insert* insert(){
//
//	int valor_key;
//	char nombre_tabla[20];
//	char value[100];
//	int timestamp;
//
//	printf("\nIngrese nombre de la tabla: ");
//	scanf("%s", &nombre_tabla);
//	printf("\nIngrese el valor de la key: ");
//	scanf("%d", &valor_key);
//	printf("\nIngrese el value: ");
//	scanf("%s", &value);
//	printf("\nIngrese el timestamp: ");
//	scanf("%d", &timestamp);
//
//	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);
//	loggear_paquete_insert(paquete);
//
//	return paquete;
//}

//----------------------------------------------------LOGGEO DE PAQUETES

void loggear_paquete_select(t_paquete_select* paquete){

	t_log* logger = iniciar_logger();
	log_info(logger, "\nNuevo paquete creado\nTipo paquete: SELECT\nNombre tabla: %s\nValor KEY: %d", paquete->nombre_tabla,paquete->valor_key);
    log_destroy(logger);
}

//void loggear_paquete_insert(t_paquete_insert* paquete){
//
//	t_log* logger = iniciar_logger();
//	log_info(logger, "\nNuevo paquete creado\nTipo paquete: INSERT\nNombre tabla: %s\nValor KEY: %d\nValue del paquete: %s\nTimestamp: %d", paquete->nombre_tabla, paquete->valor_key, paquete->value, paquete->timestamp);
//    log_destroy(logger);
//}

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
