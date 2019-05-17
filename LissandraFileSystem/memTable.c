/*
 * memTable.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include "memTable.h"
nodoTablaMemTable* primer=NULL;
nodoTablaMemTable* ultimo=NULL;

t_list* crearMemTable(){
	t_list* memTable=list_create();
	return memTable;
}



bool igualNombre(void* elemento,char* nombreBuscado){
	return nombreBuscado == ((nodoTablaMemTable*)elemento)->nombreTabla;
}


bool existeDuplicado(nodoTablaMemTable* nodoTabla){ //busca nombres de tabla Duplicados
	bool _mismoNombre(void* elemento){
		return igualNombre(elemento ,nodoTabla->nombreTabla);
	}


	return list_any_satisfy(memTable,_mismoNombre);


}

nodoTablaMemTable* crearNodoTabla(char* nombreTabla){
	nodoTablaMemTable* nuevo= malloc(sizeof(nodoTablaMemTable));
	nuevo->nombreTabla=nombreTabla;
	nuevo->registros = list_create();
return nuevo;
}


void mostrarTablas(void* elemento){
	printf("%s\t",((nodoTablaMemTable*)elemento)->nombreTabla);
	list_iterate(((nodoTablaMemTable*)elemento)->registros,mostrarRegistros);
	printf("\n");

}

void mostrarRegistros(void* elemento){
	printf(">%d %s %lli  ",((nodoRegistroMemTable*)elemento)->key,((nodoRegistroMemTable*)elemento)->value,((nodoRegistroMemTable*)elemento)->timestamp);
}


//********************- FUNCIONES IMPRIMIR -*****************************
void imprimirListaTablas(){
	list_iterate(memTable,mostrarTablas);
}

void imprimirRegistrosTabla(nodoTablaMemTable* nodoTabla){
	bool _mismoNombre(void* elemento){
			return igualNombre(elemento ,nodoTabla->nombreTabla);
		}

	nodoTablaMemTable* tablaBuscada = list_find(memTable,_mismoNombre);
	list_iterate(tablaBuscada->registros,mostrarRegistros);

}

//************************************************************************



void eliminarNodoTabla(){ //elimina el ultimo
	int ultimoElemento = memTable->elements_count-1;
	list_remove(memTable, ultimoElemento);
}


nodoRegistroMemTable* crearNodoRegistro(char*value,uint16_t key,long long timestamp){
	nodoRegistroMemTable* nuevo= malloc(sizeof(nodoRegistroMemTable));
	nuevo->value=value;
	nuevo->key=key;
	nuevo->timestamp=timestamp;
	return nuevo;
}

//********************- FUNCIONES AGREGAR -*****************************

void agregarTabla(nodoTablaMemTable* nodoTabla,nodoRegistroMemTable* nodoRegistro){
	if(memTable==NULL){
		memTable=list_create();
	}

	if(!existeDuplicado(nodoTabla)){
		  list_add(memTable, nodoTabla);}

	agregarRegistro(nodoTabla,nodoRegistro);

}

void agregarRegistro(nodoTablaMemTable* nodoTabla, nodoRegistroMemTable* nodoRegistro){
	bool _mismoNombre(void* elemento){
		return igualNombre(elemento ,nodoTabla->nombreTabla);
	}
		nodoTablaMemTable* tablaBuscada = list_find(memTable,_mismoNombre);
		list_add(tablaBuscada->registros, nodoRegistro);
}

//************************************************************************




void eliminarUltimoRegistro(nodoTablaMemTable* nodoTabla){
	bool _mismoNombre(void* elemento){
			return igualNombre(elemento ,nodoTabla->nombreTabla);
		}
	nodoTablaMemTable* tablaBuscada = list_find(memTable,_mismoNombre);
	int posicionUltimoRegistro= tablaBuscada->registros->elements_count-1;
	list_remove(tablaBuscada->registros, posicionUltimoRegistro);

}


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
