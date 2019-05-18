/*
 * memTable.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include "memTable.h"


t_list* crearMemTable(){
	t_dictionary* memTable=dictionary_create();
	return memTable;
}


void agregarTabla(t_paquete_insert* paquete_insert){
	nodoRegistroMemTable*nodoRegistro= crearNodoRegistro(paquete_insert->value,paquete_insert->valor_key,paquete_insert->timestamp);
	if(memTable==NULL){
		memTable=crearMemTable();
	}
	if(!dictionary_has_key(memTable, paquete_insert->nombre_tabla)){
		t_list *nuevo = list_create();
		dictionary_put(memTable, paquete_insert->nombre_tabla, agregarRegistro(nuevo, nodoRegistro));
	}else{
		dictionary_put(memTable, paquete_insert->nombre_tabla, agregarRegistro(dictionary_get(memTable, paquete_insert->nombre_tabla), nodoRegistro));
	}

}


void imprimirListaTablas(){
	system("clear");
	dictionary_iterator(memTable,mostrarTablas);
}


void mostrarTablas(char*key,void* elemento){
	printf("%s  ",key);
	list_iterate((t_list*) elemento,mostrarRegistros);
	printf("\n");

}


void mostrarRegistros(void* elemento){
	printf(">%d %s %lli  ",((nodoRegistroMemTable*)elemento)->key,((nodoRegistroMemTable*)elemento)->value,((nodoRegistroMemTable*)elemento)->timestamp);
}



nodoRegistroMemTable* crearNodoRegistro(char*value,uint16_t key,long long timestamp){
	nodoRegistroMemTable* nuevo= malloc(sizeof(nodoRegistroMemTable));
	nuevo->value=malloc(string_length(value)+1);
	strcpy(nuevo->value,value);
	nuevo->key=key;
	nuevo->timestamp=timestamp;
	return nuevo;
}


t_list* agregarRegistro(t_list* listaTabla, nodoRegistroMemTable* nodoRegistro){

	list_add(listaTabla,nodoRegistro);
	return listaTabla;
}

t_registro* buscarMemTable(char* nombreTabla,int key){
	bool _filtradoKey(void* elemento){
		return filtrarPorKey(elemento,key);
	}

	t_list* listaRegistrosKey = list_sorted(
								list_filter(
								dictionary_get(memTable,nombreTabla),_filtradoKey),mayorTimeStamp);
	if(!listaVacia(listaRegistrosKey)){
	nodoRegistroMemTable* registroEncontrado =(nodoRegistroMemTable*) list_remove(listaRegistrosKey, 0); // 0 es el primero
	t_registro* registroKey = crearRegistro(registroEncontrado->value,registroEncontrado->key,registroEncontrado->timestamp);
	list_clean(listaRegistrosKey);
	return registroKey;
	}
	return NULL;
}

bool listaVacia(t_list* lista){
	return list_is_empty(lista);
}

bool filtrarPorKey(void* elemento,int key){
	return ((nodoRegistroMemTable*) elemento)->key == key;
}

bool mayorTimeStamp(void*elemento1,void*elemento2){
	return ((nodoRegistroMemTable*) elemento1)->timestamp > ((nodoRegistroMemTable*) elemento2)->timestamp;
}

//void eliminarUltimoRegistro(nodoTablaMemTable* nodoTabla){
//	bool _mismoNombre(void* elemento){
//			return igualNombre(elemento ,nodoTabla->nombreTabla);
//		}
//	nodoTablaMemTable* tablaBuscada = list_find(memTable,_mismoNombre);
//	int posicionUltimoRegistro= tablaBuscada->registros->elements_count-1;
//	list_remove(tablaBuscada->registros, posicionUltimoRegistro);
//
//}
//

//
//
//
//
//void dump(){   // prototipo
//nodoTablaMemTable* auxiliar = primer;
//while(auxiliar!=NULL){
//	while(auxiliar->primerRegistro!=NULL){
//		//pasa el registro a un temporal con el nombre del temporal eg PasarATemporal(primer->nombreTabla, cant_Dumpeos) paso el nombre tabla para poder saber el nombre del temporal y la cant de dumpeos tambien Tabla1_cant_Dumpeos.tmp
//		eliminarPrimerRegistro(auxiliar);
//	}
//	free(auxiliar);
//}
//
//
//
//
//
//
//}
