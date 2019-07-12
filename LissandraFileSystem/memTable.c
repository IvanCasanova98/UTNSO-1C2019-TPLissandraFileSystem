/*
 * memTable.c
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */
#include "memTable.h"


t_dictionary* crearMemTable(){
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
		//list_destroy(nuevo);
	}else{
		dictionary_put(memTable, paquete_insert->nombre_tabla, agregarRegistro(dictionary_get(memTable, paquete_insert->nombre_tabla), nodoRegistro));
	}

}


void imprimirListaTablas(){
	//system("clear");
	dictionary_iterator(memTable,mostrarTablas);
}


void mostrarTablas(char*key,void* elemento){

	printf("%s%s%s  ",CYAN,key,MAGENTA);
	list_iterate((t_list*) elemento,mostrarRegistros);
	printf("%s\n",NORMAL_COLOR);


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
	if(memTable==NULL) return NULL;
	if(dictionary_has_key(memTable,nombreTabla)){
		if(list_any_satisfy(dictionary_get(memTable,nombreTabla),_filtradoKey)){
			t_list* listaRegistrosKey=list_create();
	t_list* listaFiltrada=list_filter(dictionary_get(memTable,nombreTabla),_filtradoKey);
	t_list* listaOrdenada=list_sorted(listaFiltrada,mayorTimeStamp);
	list_add_all(listaRegistrosKey, listaOrdenada);
	if(!listaVacia(listaRegistrosKey)){
	nodoRegistroMemTable* registroEncontrado =(nodoRegistroMemTable*) list_get(listaRegistrosKey, 0); // 0 es el primero
	t_registro* registroKey = crearRegistro(registroEncontrado->value,registroEncontrado->key,registroEncontrado->timestamp);
	//list_clean_and_destroy_elements(listaRegistrosKey,liberarNodo);
	list_destroy(listaRegistrosKey);
	list_destroy(listaOrdenada);
	list_destroy(listaFiltrada);
	//liberarNodo(registroEncontrado);
	return registroKey;
			}
		}
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
	char* arrayBloques= malloc(2+4*cantBloques);
	strcpy(arrayBloques,"[");
	int i=0;
	while(i<cantBloques){

	char* numeroBloque=	string_itoa(bloques[i]);
	strcat(arrayBloques,numeroBloque);
	i++;
	if (i==cantBloques) {free(numeroBloque);break;}
	strcat(arrayBloques,",");
	free(numeroBloque);
	}
	strcat(arrayBloques,"]");

	return arrayBloques;
}

void crearArchivotmp(char* nombreTabla, int size, int*bloques,int cantBloques){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioTemporalNuevo=DirectorioDeTemporalNuevo(nombreTabla);
	FILE *particionBin;

	char* arrayBloques=crearArrayBloques(bloques,cantBloques);

	particionBin=fopen(directorioTemporalNuevo,"w");
	fprintf(particionBin,"SIZE=%d\nBLOCKS=%s",size,arrayBloques);
	free(arrayBloques);
	free(directorioMetadata);
	free(directorioTemporalNuevo);
	fclose(particionBin);


}



void* crearTemporal(char* nombreTabla,t_list* registros){  //dictionary_iterator(memTable, crearTemporal);

	int cantidadTotalRegistros= list_size(registros);
	int i=0;
	//registros =list_map(registros, void*(*transformer)(void*));
	t_list* listaDeRegistros=list_create();
	while(i<cantidadTotalRegistros){
		nodoRegistroMemTable* nodoASerializar= list_get(registros, i);
		char * nodoSerializado= serializarRegistro(((nodoRegistroMemTable*) nodoASerializar)->value,((nodoRegistroMemTable*) nodoASerializar)->key,((nodoRegistroMemTable*) nodoASerializar)->timestamp);
		//printf("%s\n",nodoSerializado);
		list_add(listaDeRegistros, nodoSerializado);

		i++;
	}
	list_clean_and_destroy_elements(registros, liberarNodo);
	list_destroy(registros);
	char* directorioMetadata=DirectorioDeMetadata();
	t_config* config = config_create(directorioMetadata);
	int sizeBloque = atoi(config_get_string_value(config,"BLOCK_SIZE"));
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));

	int sizeTotal= sizeTotalLista(listaDeRegistros);
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


	free(directorioMetadata);
	config_destroy(config);
	int registroCharTotales=list_size(listaDeRegistros);


	int desplazamiento=0;
	int bloquesEnUso=0;
	int registrosCargados=0;
	while(registrosCargados<registroCharTotales){



		int bytesEnBloque=tamanioBloque(bloquesDisponibles[bloquesEnUso]);


		//printf("%d\n",bytesEnBloque);

		char* registroActual= list_get(listaDeRegistros, registrosCargados);
		//printf("%s\n",registroActual);
		//int tamanioRegistro = string_length(registroActual) +1;
		desplazamiento=cargarRegistro(registroActual,bytesEnBloque,bloquesDisponibles,bloquesEnUso);


		bloquesEnUso = bloquesEnUso + desplazamiento;

		//bytesEnBloque=tamanioBloque(bloquesDisponibles[bloquesEnUso]);
		//printf("%d\n",bytesEnBloque);

		registrosCargados++;

		//desplazamiento=0;

	}

	list_clean_and_destroy_elements(listaDeRegistros, liberarBloque);
	list_destroy(listaDeRegistros);

}



int cargarRegistro(char* registroActual,int bytesEnBloque, int* bloquesDisponibles,int bloquesEnUso){
	char* directorioMetadata=DirectorioDeMetadata();
	t_config* config2 = config_create(directorioMetadata);
	int tamanioBloque = atoi(config_get_string_value(config2,"BLOCK_SIZE"));
	int tamanioRegistro = string_length(registroActual) +1;
	int desplazamiento=0;

	if((tamanioRegistro+bytesEnBloque)>tamanioBloque){

	int tamanioQueEntra=abs(tamanioBloque-bytesEnBloque);
	char* parteQueEntra= malloc(tamanioQueEntra+1);
	char* stringQueEntra=string_substring(registroActual, 0,tamanioQueEntra);
	strcpy(parteQueEntra,stringQueEntra);
	escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueEntra,0);
	char* parteQueNoEntra= malloc(tamanioRegistro-tamanioQueEntra+1);
	char* stringQueNoEntra= string_substring_from(registroActual,tamanioQueEntra);
	strcpy(parteQueNoEntra,stringQueNoEntra);
	bloquesEnUso++;
	desplazamiento++;
	tamanioRegistro= tamanioRegistro-tamanioQueEntra;
	free(stringQueEntra);
	free(stringQueNoEntra);
	while(tamanioRegistro>tamanioBloque){
		char* stringQueEntra=string_substring(parteQueNoEntra, 0,tamanioBloque);
		strcpy(parteQueEntra,stringQueEntra);
		escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueEntra,0);
		char* stringQueNoEntra= string_substring_from(parteQueNoEntra,tamanioBloque);
		strcpy(parteQueNoEntra,stringQueNoEntra);
		bloquesEnUso++;
		desplazamiento++;
		tamanioRegistro= tamanioRegistro-tamanioBloque;
		free(stringQueEntra);
		free(stringQueNoEntra);
	}
	escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueNoEntra,1);
	free(parteQueEntra);
	free(parteQueNoEntra);

//		free(directorioMetadata);
//		config_destroy(config2);
//		desplazamiento+=cargarRegistro(parteQueNoEntra,tamanioBloque,bloquesDisponibles,bloquesEnUso);
	}else{

	escribirEnBloque(bloquesDisponibles[bloquesEnUso], registroActual,1);

	}
	free(directorioMetadata);
	config_destroy(config2);
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


void* dump(){
	sem_wait(&SemaforoMemtable);
	LogDumpeo();
	dictionary_iterator(memTable, crearTemporal);
	dictionary_destroy(memTable);
	memTable=NULL;
	estaDump=0;
	sem_post(&SemaforoMemtable);
}

void chekearDumpeo(){
	//pthread_mutex_lock(dictionary_get(TablasSem,"PERSONAS"));
			pthread_t dumpeo;
			t_config* config = leer_config();
			int tiempoDump = atoi(config_get_string_value(config, "TIEMPO_DUMP"));
			struct timeval verificar;
			//gettimeofday(&verificar,NULL);
			//int tiempoTranscurrido = ((double)(verificar.tv_sec - tiempoHastaDump.tv_sec) + (double)(verificar.tv_usec - tiempoHastaDump.tv_usec)/1000000)*1000;

//			gettimeofday(&tiempoHastaDump,NULL);
			while(1){

				if((tiempoDump *1000)<1000000)
					usleep(tiempoDump*1000);
				else
					sleep(tiempoDump/1000);
					if(!estaDump  && !(memTable==NULL)){
						estaDump=1;
						pthread_create(&dumpeo,NULL,dump,NULL);
						pthread_join(dumpeo, (void**)NULL);
						//gettimeofday(&tiempoHastaDump,NULL);
						//deployMenu();
					}
			}
//			else{
//			gettimeofday(&tiempoHastaDump,NULL);}
			config_destroy(config);

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
