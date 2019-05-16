/*
 * memTable.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include "memTable.h"

nodoTablaMemTable* primer=NULL;
nodoTablaMemTable* ultimo=NULL;

void agregarTabla(nodoTablaMemTable* nodoTabla){
	if(primer==NULL){
		primer=nodoTabla;
		ultimo=nodoTabla;
	}else{
		if(!existeDuplicado(nodoTabla))
		ultimo -> siguiente = nodoTabla;
		ultimo = nodoTabla;

	}
}

int existeDuplicado(nodoTablaMemTable* nodoTabla){ //busca nombres de tabla Duplicados
	nodoTablaMemTable* auxiliar;
	auxiliar=primer;
	while(auxiliar!=NULL){
		if(auxiliar->nombreTabla==nodoTabla->nombreTabla){ return 1;}
		else { auxiliar= auxiliar->siguiente;}
	}
	return 0;
}

nodoTablaMemTable* crearNodoTabla(char* nombreTabla){
	nodoTablaMemTable* nuevo= malloc(sizeof(nodoTablaMemTable));
	nuevo->nombreTabla=nombreTabla;
	nuevo->siguiente=NULL;
	nuevo->primerRegistro=NULL;
	nuevo->ultimoRegistro=NULL;
	return nuevo;
}


void imprimirListaTablas(){
	nodoTablaMemTable* auxiliar;
	auxiliar=primer;
	while(auxiliar!=NULL){
		printf("%s\n",auxiliar->nombreTabla);
		auxiliar= auxiliar->siguiente;}

}

void eliminarNodoTabla(nodoTablaMemTable* nodoTabla){
	nodoTablaMemTable* auxiliar;
	auxiliar=primer;
	if(auxiliar==nodoTabla){
		primer=primer->siguiente;
	free(nodoTabla);
	}else{
	while(auxiliar->siguiente!=nodoTabla){
		auxiliar= auxiliar->siguiente;
	}
	nodoTablaMemTable* auxiliar2=nodoTabla->siguiente;
	auxiliar->siguiente= auxiliar2;
	free(nodoTabla);
	}
}

nodoRegistroMemTable* crearNodoRegistro(char*value,uint16_t key,long long timestamp){
	nodoRegistroMemTable* nuevo= malloc(sizeof(nodoRegistroMemTable));
	nuevo->value=value;
	nuevo->key=key;
	nuevo->timestamp=timestamp;
	nuevo->siguienteRegistro=NULL;
	return nuevo;
}


void agregarRegistro(nodoTablaMemTable* nodoTabla, nodoRegistroMemTable* nodoRegistro){
	nodoTablaMemTable* auxiliar = primer;
	int i=1;
	while(auxiliar!=NULL && i==1){
			if(auxiliar==nodoTabla){
				if(auxiliar->primerRegistro==NULL){

				auxiliar->primerRegistro=nodoRegistro;
				auxiliar->ultimoRegistro=nodoRegistro;
				i=0;
			}else{
					auxiliar->ultimoRegistro->siguienteRegistro=nodoRegistro;
					auxiliar->ultimoRegistro =nodoRegistro;
					i=0;
				}

			}else{
			auxiliar= auxiliar->siguiente;
			}
	}



}

void imprimirRegistrosTabla(nodoTablaMemTable* nodoTabla){
	nodoTablaMemTable* auxiliar = primer;
	nodoRegistroMemTable* RegistroPrimer= auxiliar->primerRegistro;
	int i=1;
	while(auxiliar!=NULL && i){
		if(auxiliar==nodoTabla){
			printf("Tabla: %s ",nodoTabla->nombreTabla);
			while(auxiliar->primerRegistro!=NULL){
				printf("Value:%s Key:%d TimeStamp:%d\n",auxiliar->primerRegistro->value,auxiliar->primerRegistro->key,auxiliar->primerRegistro->timestamp);
				auxiliar->primerRegistro = auxiliar->primerRegistro->siguienteRegistro;
			}
			i=0;
		}

		auxiliar=auxiliar->siguiente;
		//RegistroPrimer= auxiliar->primerRegistro;
	}


}

