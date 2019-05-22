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
	list_clean_and_destroy_elements(listaRegistrosKey,liberarNodo);
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

char* crearArrayBloques(int*bloques,int cantBloques){
	char* arrayBloques= malloc(2+2*cantBloques-1);
	strcpy(arrayBloques,"[");
	int i=0;
	while(i<cantBloques){
	strcat(arrayBloques,string_itoa(bloques[i]));
	i++;
	if (i==cantBloques) break;
	strcat(arrayBloques,",");
	}
	strcat(arrayBloques,"]");
	return arrayBloques;
}

void crearArchivotmp(char* nombreTabla, int size, int*bloques,int cantBloques){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioTemporal=DirectorioDeTemporal(nombreTabla);
	FILE *particionBin;

	char* arrayBloques=crearArrayBloques(bloques,cantBloques);

	particionBin=fopen(directorioTemporal,"w");
	fprintf(particionBin,"SIZE=%d\nBLOCKS=%s",size,arrayBloques);
	free(arrayBloques);
	free(directorioMetadata);
	free(directorioTemporal);
	fclose(particionBin);


}

void crearTemporal(char* nombreTabla,t_list* registros){  //dictionary_iterator(memTable, crearTemporal);

	int cantidadTotalRegistros= list_size(registros);
	int i=0;
	while(i<cantidadTotalRegistros){
		nodoRegistroMemTable* nodoASerializar= list_get(registros, i);
		char * nodoSerializado= serializarRegistro(((nodoRegistroMemTable*) nodoASerializar)->value,((nodoRegistroMemTable*) nodoASerializar)->key,((nodoRegistroMemTable*) nodoASerializar)->timestamp);
		list_replace_and_destroy_element(registros, i, nodoSerializado, liberarNodo);
		i++;
	}
	char* directorioMetadata=DirectorioDeMetadata();
	t_config* config = config_create(directorioMetadata);
	int sizeBloque = atoi(config_get_string_value(config,"BLOCK_SIZE"));
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));

	int sizeTotal= sizeTotalLista(registros);
	int nroBloques;

	if(sizeTotal%sizeBloque==0){
		nroBloques = sizeTotal/sizeBloque;
	}else{nroBloques = trunc((sizeTotal/sizeBloque)) +1;}


	int bloquesDisponibles[nroBloques];
	i=0;
	int bloquesOcupados=0;
	while(bloquesOcupados<nroBloques){
		while(i<blockNum){
			if(!bitarray_test_bit(bitmap,i)){
				bitarray_set_bit(bitmap,i);
				crearBloque(i);
				//pruebasSet();
				break;
			}
		i++;
		}
	bloquesDisponibles[bloquesOcupados]=i;
	bloquesOcupados++;
	}


	crearArchivotmp(nombreTabla, sizeTotal, bloquesDisponibles,nroBloques);





	int bloquesEnUso=0;
	int registrosCargados=0;
	while(registrosCargados<cantidadTotalRegistros){
		int bytesEnBloque=tamanioBloque(bloquesDisponibles[bloquesEnUso]);
		char* registroActual= list_take(registros, registrosCargados);
		int tamanioRegistro = string_length(registroActual) +1;
		int desplazamiento=cargarRegistro(registroActual,bytesEnBloque,bloquesDisponibles,bloquesEnUso);
		bloquesEnUso = bloquesEnUso + desplazamiento;
		registrosCargados++;



	}
	free(directorioMetadata);
	list_clean_and_destroy_elements(registros, liberarBloque);
	config_destroy(config);

}



int cargarRegistro(char* registroActual,int bytesEnBloque, int* bloquesDisponibles,int bloquesEnUso){
	char* directorioMetadata=DirectorioDeMetadata();
	t_config* config = config_create(directorioMetadata);
	int tamanioBloque = atoi(config_get_string_value(config,"BLOCK_SIZE"));
	int tamanioRegistro = string_length(registroActual) +1;
	int desplazamiento=0;
	if(tamanioRegistro+bytesEnBloque>tamanioBloque){
		int tamanioQueEntra=abs(tamanioBloque-(tamanioRegistro+bytesEnBloque));
		char* parteQueEntra= string_substring(registroActual, 0,tamanioQueEntra);
		escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueEntra,0);
		char* parteQueNoEntra= string_substring_from(registroActual,tamanioQueEntra);
		desplazamiento++;
		free(directorioMetadata);
		config_destroy(config);
		cargarRegistro(parteQueNoEntra,tamanioBloque,bloquesDisponibles,bloquesEnUso++);
	}else{
	escribirEnBloque(bloquesDisponibles[bloquesEnUso], registroActual,1);
	free(directorioMetadata);
	config_destroy(config);
	}

	return desplazamiento;
}

void liberarNodo(void* nodo){
	free(((nodoRegistroMemTable*) nodo)->value);
	free((nodoRegistroMemTable*) nodo);
}

int sumatoriaSize(int numeroTotal,void*elemento1){
	//printf("o");
	return numeroTotal=numeroTotal+ (strlen(elemento1)+1);
}

int sizeTotalLista(t_list* registros){
	return list_fold(registros,0,sumatoriaSize);
}

//int sizeTotalLista(t_list* registros){
//	int sizeLista= list_size(registros);
//	int i=0;
//	int size=0;
//	while(i<sizeLista){
//		size=size + (strlen(list_get(registros, i))+1);
//	i++;
//	}
//	return size;
//}

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
