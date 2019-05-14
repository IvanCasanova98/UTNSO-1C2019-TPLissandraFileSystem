#ifndef MEMTABLE_H_
#define MEMTABLE_H_
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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

/*
typedef struct lista_de_KeyValue_Timestamp
{
	uint16_t valor_key;
	long long timestamp;
	struct lista_de_KeyValue_Timestamp *anterior;
	struct lista_de_KeyValue_Timestamp *siguiente;
}lista_de_KeyValue_Timestamp;
*/

//typedef struct t_KeyValue_Timestamp{
//	uint16_t valor_key;
//	long long timestamp;
//}t_KeyValue_Timestamp;

long long SELECT(char *nombre_tabla, uint16_t valor_key);
void INSERT(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp);



/*
 * SE UTILIZA EL TYPEDEF PARA EVITAR ESTAR UTILIZANDO struct nodo_registro_tmp
 * o struct nodo_tabla_tmp, para todo.
 */

typedef nodo_registro_tmp *ptr_nodo_registro;
typedef nodo_tabla_tmp *ptr_nodo_tabla;

//typedef lista_de_KeyValue_Timestamp *ptr_KeyValue_Timestamp;

long long Buscar_KeyValue_conMayor_TimeStamp(ptr_nodo_tabla TablaBuscada,uint16_t valor_key);
ptr_nodo_registro CargarRegistrosAUnNodo(uint16_t valor_key, char *value, long long timestamp);
ptr_nodo_tabla BuscarLaTabla(ptr_nodo_tabla TablaInicial, char *nombre_tabla);
ptr_nodo_tabla CrearTabla(char *nombre_tabla);
void InsertarleRegistroATablaAlFinal(ptr_nodo_registro nuevoRegistro, ptr_nodo_tabla TablaDeseada);
void InsertarTablaAlFinal(ptr_nodo_registro registroDeLaNuevaLista,ptr_nodo_tabla TablaInicial,ptr_nodo_tabla TablaAInsertar);

//FUNCION PARA PROBAR
void imprimirRegistrosVALUEDe(char *nombre_tabla);




#endif /* MEMTABLE_H_ */
