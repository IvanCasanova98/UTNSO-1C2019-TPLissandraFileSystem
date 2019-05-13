#ifndef MEMTABLE_H_
#define MEMTABLE_H_

#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<alloc.h>

typedef struct nodo_registro_tmp
{
	uint16_t valor_key;
	char *value;
	long long timestamp;
	struct nodo_registro_tmp *sgte_registro;
}nodo_registro_tmp;


typedef struct nodo_tabla_tmp
{
	char* nombre_tabla;
	nodo_registro_tmp *sgte_registro;
	struct nodo_tabla_tmp *sgte_tabla;
}nodo_tabla_tmp;

typedef struct lista_de_KeyValue_Timestamp
{
	uint16_t valor_key;
	long long timestamp;
	struct lista_de_KeyValue_Timestamp *sgte;
}lista_de_KeyValue_Timestamp;

uint16_t SELECT(char *nombre_tabla, uint16_t valor_key);
void INSERT(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp);



/*
 * SE UTILIZA EL TYPEDEF PARA EVITAR ESTAR UTILIZANDO struct nodo_registro_tmp
 * o struct nodo_tabla_tmp, para todo.
 */

typedef nodo_registro_tmp *ptr_nodo_registro;
typedef nodo_tabla_tmp *ptr_nodo_tabla;



ptr_nodo_registro CargarRegistrosAUnNodo(uint16_t valor_key, char *value, long long timestamp);
ptr_nodo_tabla BuscarLaTabla(ptr_nodo_tabla TablaInicial, char *nombre_tabla);
ptr_nodo_tabla CrearTabla(char *nombre_tabla);
void InsertarleRegistroATablaAlFinal(ptr_nodo_registro nuevoRegistro, ptr_nodo_tabla TablaDeseada);
void InsertarTablaAlFinal(ptr_nodo_registro registroDeLaNuevaLista,ptr_nodo_tabla TablaInicial,ptr_nodo_tabla TablaAInsertar);

//FUNCION PARA PROBAR
void imprimirRegistrosVALUEDe(char *nombre_tabla);




#endif /* MEMTABLE_H_ */
