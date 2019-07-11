/*
 * compactador.c
 *
 *  Created on: 13 jun. 2019
 *      Author: utnso
 */
#include"compactador.h"



void* funcionCompactar(void* arg){
	char* nombreTabla = (char*) arg;
	pthread_t compactador;
	char* DireccionmetaDataTabla = DirectorioDeMetadataTabla(nombreTabla);
	t_config* config = config_create(DireccionmetaDataTabla);
	int TiempoCompactacion = config_get_int_value(config, "COMPACTION_TIME");
	char * DirectorioTabla = DirectorioDeTabla(nombreTabla);
	bool estaCompactando=0;
	while(1){
			if((TiempoCompactacion *1000)<1000000)
					usleep(TiempoCompactacion*1000);
				else
					sleep(TiempoCompactacion/1000);
				if(existe_temporal(DirectorioTabla)){
					if(!estaCompactando){
					estaCompactando=1;
					pthread_create(&compactador,NULL,compactar,arg);
					pthread_join(compactador, (void**)NULL);
					estaCompactando=0;
					}
				}else{
					printf("cerrando hilo tabla");
					bool _buscarSemaforo(void* elemento){
								return buscarSemaforo(elemento,nombreTabla);
					}
					list_remove_and_destroy_by_condition(ListaSem,_buscarSemaforo,borrarSemaforo);
					dictionary_remove(TablasCompactacion,nombreTabla);
					break;

				}
				}
	free(DirectorioTabla);
	free(DireccionmetaDataTabla);
	//free(nombreTabla);
}


void* compactar(void * arg){
	char* nombreTabla= (char*) arg;
	bool _buscarSemaforo(void* elemento){
				return buscarSemaforo(elemento,nombreTabla);
	}
	semaforoTabla* Tablasemaforo = list_find(ListaSem,_buscarSemaforo);
	char * DirectorioTabla = DirectorioDeTabla(nombreTabla);
	char* primerTemporal = DirectorioDeTemporal(nombreTabla,1);
	char* DireccionmetaDataTabla = DirectorioDeMetadataTabla(nombreTabla);
	t_config* config = config_create(DireccionmetaDataTabla);
	int NumeroParticiones = config_get_int_value(config, "PARTITIONS");
	int TiempoCompactacion = config_get_int_value(config, "COMPACTION_TIME");
//	bool estaCompactando=0;
//	while(1){
//		sleep(TiempoCompactacion/1000);
//	if(!existe_temporal(DirectorioTabla)) break; //POR SI SE HACE UN DROP existe_temporal ES LO MISMO QUE EXISTE TABLA

	if(existe_temporal(primerTemporal))
	{
		sem_wait(&(Tablasemaforo->semaforoCompactacion));
		//estaCompactando=1;
		struct timeval tiempoAntesDeCompactar , tiempoDespuesDeCompactar;

		cambiarNombreTmpATmpc(nombreTabla);
		t_list* ListaRegistrosTemporales= LiberarTmpc(nombreTabla);


		LogCompactacion(nombreTabla);
		t_list* ListaRegistrosTemporalCompactados[NumeroParticiones];
		t_list* RegistrosACargar[NumeroParticiones];
		t_list* ListaRegistrosParticion[NumeroParticiones];


		for(int i =0; i<NumeroParticiones;i++)
		{

			bool _filtradoMismaKey(void* elemento){
					return filtrarPorMismaKey(elemento,i,NumeroParticiones);
			}

			ListaRegistrosParticion[i] = LiberarBin(nombreTabla,i);
			ListaRegistrosTemporalCompactados[i]= list_filter(ListaRegistrosTemporales, _filtradoMismaKey);
			RegistrosACargar[i] =ActualizarRegistrosParticion(ListaRegistrosParticion[i],ListaRegistrosTemporalCompactados[i]);

			}



		gettimeofday(&tiempoAntesDeCompactar,NULL);
		// ACA COMPACTA




		//printf("%s \n",Tablasemaforo->nombreTabla);
		sem_wait(&(Tablasemaforo->semaforoTabla));
		//dictionary_get(TablasSem,nombreTabla);
		for(int i =0; i<NumeroParticiones;i++){
			if(!list_is_empty(RegistrosACargar[i])){
				char* TodosLosRegistrosActualizados= crearRegistroACargar(RegistrosACargar[i]);

				RemoverParticionDeTablaEspecificaYSusBloques(nombreTabla,i);

				crearParticionNueva(nombreTabla,TodosLosRegistrosActualizados,i);

				free(TodosLosRegistrosActualizados);
				}

			//list_destroy(RegistrosACargar[i]);


			}

		sem_post(&(Tablasemaforo->semaforoTabla));

			//free(TodosLosRegistrosActualizados);

//			list_destroy(ListaRegistrosParticion);
//			list_destroy(ListaRegistrosTemporalCompactados);
//			list_destroy(RegistrosACargar);
//			list_destroy_and_destroy_elements(ListaRegistrosParticion,liberarRegistro); //Rompe
		// list_destroy_and_destroy_elements(ListaRegistrosParticion,liberarRegistro);
		//list_destroy_and_destroy_elements(ListaRegistrosParticion,liberarRegistro);


		//list_destroy_and_destroy_elements(ListaRegistrosTemporales,liberarRegistro);
		gettimeofday(&tiempoDespuesDeCompactar,NULL);
		int tiempoTranscurrido = ((double)(tiempoDespuesDeCompactar.tv_sec - tiempoAntesDeCompactar.tv_sec) + (double)(tiempoDespuesDeCompactar.tv_usec - tiempoAntesDeCompactar.tv_usec)/1000000)*1000;
		LogCompactacionTerminada(nombreTabla,tiempoTranscurrido);
		sem_post(&(Tablasemaforo->semaforoCompactacion));
		//estaCompactando=0;
	}

	//free(nombreTabla);
	config_destroy(config);
	free(DireccionmetaDataTabla);
	free(primerTemporal);
	free(DirectorioTabla);
}

bool buscarSemaforo(void* elemento,char* nombreTabla){
	return !strcmp((((semaforoTabla*) elemento)->nombreTabla),nombreTabla);
}


void cambiarNombreTmpATmpc(char* nombretabla){ // 9ml
		t_config* config = config_create("Lissandra.config");
		char* NombreDadoVuelta = string_reverse(nombretabla);
		char* NombreTmp = string_substring_until(NombreDadoVuelta, 1);


		char buffer [3];
		char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");

		char* directorioAuxNuevo= malloc(strlen(Montaje) + strlen(".tmpc") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
		char* directorioAuxViejo= malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
		char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
		strcpy(directorioTablas,Montaje);
		strcat(directorioTablas,"Tables/");
		strcat(directorioTablas,nombretabla);
		strcat(directorioTablas,"/");
		strcat(directorioTablas,NombreTmp);
		int i =1;
		strcpy(directorioAuxViejo,directorioTablas);

		sprintf(buffer,"%d",i);
		strcat(directorioAuxViejo,buffer);
		strcat(directorioAuxViejo,".tmp");
		while(existe_temporal(directorioAuxViejo)){



			strcpy(directorioAuxNuevo,directorioTablas);
			sprintf(buffer,"%d",i);
			strcat(directorioAuxNuevo,buffer);
			strcat(directorioAuxNuevo,".tmpc");

			rename(directorioAuxViejo,directorioAuxNuevo);

			i++;

			strcpy(directorioAuxViejo,directorioTablas);
			sprintf(buffer,"%d",i);
			strcat(directorioAuxViejo,buffer);

			strcat(directorioAuxViejo,".tmp");



		}
		free(NombreTmp);
		free(NombreDadoVuelta);
		free(directorioAuxNuevo);
		free(directorioAuxViejo);
		free(directorioTablas);
		config_destroy(config);
}

void* imprimirRegistro(void*registro){
	t_registro* registroIngresado = (t_registro*) registro;
	printf("%s%lli;%d;%s%s\n",CYAN, registroIngresado->timestamp,registroIngresado->key,registroIngresado->value,NORMAL_COLOR);


}




t_list* ActualizarRegistrosParticion(t_list* ListaRegistrosParticion,t_list* ListaRegistrosTemporalCompactados){


	void* _CompararRegistros(void*registro){
		CompararRegistros(registro,ListaRegistrosParticion);
	}

	list_iterate(ListaRegistrosTemporalCompactados,_CompararRegistros);

	return ListaRegistrosParticion;

}

void* CompararRegistros(void*registroTemporal,t_list* ListaRegistrosParticion){

	t_registro* registroTemporalCasteado= (t_registro*)registroTemporal;

	bool _EstaLaKey(void*registro){
		return EstaLaKey(registro,(t_registro*)registroTemporal);
	}

	if(list_any_satisfy(ListaRegistrosParticion, _EstaLaKey))
	{

		bool _CambiarRegistro(void*registroParticion){
			return CambiarRegistro(registroParticion,registroTemporal);
		}

		int sizeActual =list_size(ListaRegistrosParticion);
		list_remove_by_condition(ListaRegistrosParticion,_CambiarRegistro);
		int sizeNuevo=list_size(ListaRegistrosParticion);
		if(sizeNuevo != sizeActual){
			list_add(ListaRegistrosParticion,registroTemporal);
		}else liberarRegistro(registroTemporalCasteado);


	}else
	{
		list_add(ListaRegistrosParticion,registroTemporal);
	}

}

bool CambiarRegistro(void*registroParticion,void*registroTemporal){
	t_registro* RegistroTemporalCasteado= (t_registro*)registroTemporal;
	t_registro* RegistroParticionCasteado = (t_registro*)registroParticion;

	if(RegistroParticionCasteado->key==RegistroTemporalCasteado->key){
		if(RegistroParticionCasteado->timestamp < RegistroTemporalCasteado->timestamp){
			liberarRegistro(registroParticion);
			return 1;
		}

	}
	liberarRegistro(registroTemporal);
	return 0;
}



bool EstaLaKey(void* registro ,t_registro* registroTemporalCasteado){
	return ((t_registro*) registro)->key == registroTemporalCasteado->key;


}

bool filtrarPorMismaKey(void* registro,int nroParticion,int NumeroParticiones){


	return (((t_registro*) registro)->key % NumeroParticiones) == nroParticion;

}

t_list* LiberarTmpc(char *nombreTabla){

	int temporales=1;
	char* directorioTemporal= DirectorioDeTemporalCompactacion(nombreTabla,temporales);
	char* registrosCompletos=string_new();
	while(existe_temporal(directorioTemporal))
	{
		int i=0;
		t_config* config = config_create(directorioTemporal);
		char** arrayBloques = config_get_array_value(config, "BLOCKS");
		while(arrayBloques[i]){
			int nroBloque = atoi(arrayBloques[i]);
			char* registrosDeBloque= ObtenerContenidoBloque(nroBloque);
			string_append(&registrosCompletos,registrosDeBloque);
			//free(registrosDeBloque);
			i++;
		}
		config_destroy(config);
		string_iterate_lines(arrayBloques,free);
		free(arrayBloques);
		free(directorioTemporal);
		temporales++;
		directorioTemporal= DirectorioDeTemporalCompactacion(nombreTabla,temporales);
	}


	free(directorioTemporal);
	t_list* listaDeRegistros=list_create();
		int registrosRecorridos=0;
		char**arrayRegistros= string_split(registrosCompletos,"\n");
		while(arrayRegistros[registrosRecorridos]!=NULL)
		{
			char** RegistroActual =string_split(arrayRegistros[registrosRecorridos],";");
			list_add(listaDeRegistros,crearRegistro(RegistroActual[2],atoi(RegistroActual[1]),atoll(RegistroActual[0])));
			registrosRecorridos++;
			string_iterate_lines(RegistroActual,free);
			free(RegistroActual);
		}


		string_iterate_lines(arrayRegistros,free);

		free(arrayRegistros);

		free(registrosCompletos);


		RemoverTemporalesCompactadosDeTablaYSusBloques(nombreTabla);




		return listaDeRegistros;
}


void RemoverTemporalesCompactadosDeTablaYSusBloques(char* nombretabla){
	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);


	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");

	char* directorioAux= malloc(strlen(Montaje) + strlen(".tmpc") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmpc") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	int i =1;
	strcpy(directorioAux,directorioTablas);
	strcat(directorioAux,string_itoa(i));
	strcat(directorioAux,".tmpc");
	while(existe_temporal(directorioAux)){
		t_config* directorioTemporal= config_create(directorioAux);
		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorioTemporal,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		remove(directorioAux);
		config_destroy(directorioTemporal); //AGREGADO

		i++;
		strcpy(directorioAux,directorioTablas);
		strcat(directorioAux,string_itoa(i));
		strcat(directorioAux,".tmpc");
	}

	config_destroy(config); //AGREGADO
	free(directorioTablas); //AGREGADO
	free(directorioAux);
	free(NombreTmp); //AGREGADO

}


t_list* LiberarBin(char *nombreTabla,int NroBin){


	char* directorioBin= DirectorioDeParticion(nombreTabla,NroBin);
	char* registrosCompletos=string_new();

		int i=0;
		t_config* config = config_create(directorioBin);
		char** arrayBloques = config_get_array_value(config, "BLOCKS");
		while(arrayBloques[i]){
			int nroBloque = atoi(arrayBloques[i]);
			char* registrosDeBloque= ObtenerContenidoBloque(nroBloque);
			string_append(&registrosCompletos,registrosDeBloque);
//			free(registrosDeBloque);
			i++;
		}
		config_destroy(config);
		string_iterate_lines(arrayBloques,free);
		free(arrayBloques);
		free(directorioBin);


	t_list* listaDeRegistros=list_create();
		int registrosRecorridos=0;
		char**arrayRegistros= string_split(registrosCompletos,"\n");
		while(arrayRegistros[registrosRecorridos]!=NULL){
			char** RegistroActual =string_split(arrayRegistros[registrosRecorridos],";");
			list_add(listaDeRegistros,crearRegistro(RegistroActual[2],atoi(RegistroActual[1]),atoll(RegistroActual[0])));
			registrosRecorridos++;
			string_iterate_lines(RegistroActual,free);
			free(RegistroActual);
			}


		string_iterate_lines(arrayRegistros,free);

		free(arrayRegistros);

		free(registrosCompletos);



//		RemoverParticionDeTablaEspecificaYSusBloques(nombreTabla,NroBin);


		return listaDeRegistros;
}

char* crearRegistroACargar(t_list* ListaRegistros){

	char* RegistroInicial=string_new();
	t_list* listaRegistrosChar= list_map(ListaRegistros, transformarARegistroChar);
	char* todosLosRegistrosChar = list_fold(listaRegistrosChar, RegistroInicial,concatenar);


	list_destroy_and_destroy_elements(listaRegistrosChar,liberarBloque);

	return todosLosRegistrosChar;
}


void* transformarARegistroChar(void* registro){

	serializarRegistroBarraN(((t_registro*)registro)->value,((t_registro*)registro)->key,((t_registro*)registro)->timestamp);


}


char* serializarRegistroBarraN(char* value,uint16_t key,long long timestamp){

	char aux[256];
	sprintf(aux,"%lli",timestamp);
	//char* timeChar= string_itoa(timestamp);
	char* keyChar=string_itoa(key);
	char* registro = malloc(strlen(value)+strlen(aux)+strlen(keyChar)+4);

	strcpy(registro,aux);
	strcat(registro,";");
	strcat(registro,keyChar);
	strcat(registro,";");
	strcat(registro,value);
	strcat(registro,"\n");
	//printf("%s",registro);
	return registro;


}


void* concatenar(char*registrosCompletos,char* registrosDeBloque){


	string_append(&registrosCompletos,registrosDeBloque);


}


void* crearParticionNueva(char* nombreTabla,char* registros,int nroParticion){
	char* directorioMetadata=DirectorioDeMetadata();
	t_config* config = config_create(directorioMetadata);
	int sizeBloque = atoi(config_get_string_value(config,"BLOCK_SIZE"));
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));

	int sizeTotal = string_length(registros);
	int nroBloques;
	if(sizeTotal%sizeBloque==0){
		nroBloques = sizeTotal/sizeBloque;
	}else{nroBloques = trunc((sizeTotal/sizeBloque)) +1;}

	if(sizeTotal==0) nroBloques=1;

	int bloquesDisponibles[nroBloques];
	int i=0;
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

	crearArchivobin(nombreTabla,sizeTotal,bloquesDisponibles,nroBloques,nroParticion);

	free(directorioMetadata);
	config_destroy(config);

	cargarRegistros(registros,bloquesDisponibles);



}


void cargarRegistros(char* registroActual, int* bloquesDisponibles){
	char* directorioMetadata=DirectorioDeMetadata();
	t_config* config2 = config_create(directorioMetadata);
	int tamanioBloque = atoi(config_get_string_value(config2,"BLOCK_SIZE"));
	int tamanioRegistro = string_length(registroActual);
	int desplazamiento=0;
	int bloquesEnUso=0;

	if((tamanioRegistro)>tamanioBloque){

	int tamanioQueEntra=tamanioBloque;
	char* parteQueEntra= malloc(tamanioQueEntra+1);
	strcpy(parteQueEntra,string_substring(registroActual, 0,tamanioQueEntra));
	escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueEntra,0);
	char* parteQueNoEntra= malloc(tamanioRegistro-tamanioQueEntra+1);
	strcpy(parteQueNoEntra,string_substring_from(registroActual,tamanioQueEntra));
	bloquesEnUso++;
	desplazamiento++;
	tamanioRegistro= tamanioRegistro-tamanioQueEntra;

	while(tamanioRegistro>tamanioBloque){
		strcpy(parteQueEntra,string_substring(parteQueNoEntra, 0,tamanioBloque));
		escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueEntra,0);
		strcpy(parteQueNoEntra,string_substring_from(parteQueNoEntra,tamanioBloque));
		bloquesEnUso++;
		desplazamiento++;
		tamanioRegistro= tamanioRegistro-tamanioBloque;
	}
	escribirEnBloque(bloquesDisponibles[bloquesEnUso], parteQueNoEntra,0);
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

}



void crearArchivobin(char* nombreTabla, int size, int*bloques,int cantBloques,int nroParticion){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directoriobin=DirectorioDeParticion(nombreTabla,nroParticion);
	FILE *particionBin;

	char* arrayBloques=crearArrayBloques(bloques,cantBloques);

	particionBin=fopen(directoriobin,"w");
	fprintf(particionBin,"SIZE=%d\nBLOCKS=%s",size,arrayBloques);
	free(arrayBloques);
	free(directorioMetadata);
	free(directoriobin);
	fclose(particionBin);


}



void levantarHilosTablasExistentesCompactacion(){

	DIR *d;
	struct dirent *dir;
	char *directorioTablas= DirectorioDeTabla("");
	d = opendir(directorioTablas);
	int TablasYaExistentes=cantidadDeTablasExistentes();
	pthread_t compactador[TablasYaExistentes];
	//pthread_mutex_t mutex[TablasYaExistentes];
	//sem_t semaforoTabla[TablasYaExistentes];
	int i=0;
	if (d != NULL)
		TablasCompactacion=dictionary_create();
		ListaSem=list_create();
	{

		while ((dir = readdir(d)) != NULL)
		{
			if (!string_contains(dir->d_name,"."))
			{


				char* nombreTabla = malloc(sizeof(dir->d_name)+1);
				//mutex[i] = PTHREAD_MUTEX_INITIALIZER;

				//pthread_mutex_init(& mutex[i], NULL);

				strcpy(nombreTabla,dir->d_name);
				semaforoTabla* semaforoNuevo= crearSemaforo(nombreTabla);
				list_add(ListaSem,semaforoNuevo);
				dictionary_put(TablasCompactacion,nombreTabla,compactador[i]);

				//printf("%d",i);

				i++;
				free(nombreTabla);
				//pthread_join(compactador[i], (void**)NULL);
			}


		}
		dictionary_iterator(TablasCompactacion,levantarHiloCompactacion);
		free(directorioTablas);
		closedir(dir);
	closedir(d);
	}
}




void* levantarHiloCompactacion(char* nombreTablaNueva,void* compactador){
	pthread_t _compactador = (pthread_t) compactador;
	//sem_t semaforoTabla;
	//sem_init(&semaforoTabla,0,1);

	//int i=0;
	//i++;
	//printf("%d",i);
	//dictionary_put(TablasSem,nombreTablaNueva,&semaforoTabla);
	pthread_create(&_compactador,NULL,funcionCompactar,(void*)nombreTablaNueva);
	//pthread_join(&_compactador,NULL);
	//dictionary_remove(TablasCompactacion,nombreTablaNueva);

}


int cantidadDeTablasExistentes(){

	DIR *d;
	 struct dirent *dir;
	  char *directorioTablas= DirectorioDeTabla("");
	  d = opendir(directorioTablas);
	  int i =0;
	  if (d != NULL)
	  {
		while ((dir = readdir(d)) != NULL)
		{
			if (!string_contains(dir->d_name,"."))
			i++;
		}
		free(directorioTablas);
		closedir(dir);
		closedir(d);

}
	  return i;
}


void crearHiloCompactacion(char* nombreTabla){
	pthread_t compactador;


	if(dictionary_is_empty(TablasCompactacion)){
		TablasCompactacion=dictionary_create();
		ListaSem=list_create();
	}
	levantarHiloCompactacion(nombreTabla,compactador);
	dictionary_put(TablasCompactacion,nombreTabla,compactador);
	//free(nombreTabla);

}

semaforoTabla* crearSemaforo(char* nombreTabla){
	char* nombreSemaforo=malloc(strlen (nombreTabla)+1);
	strcpy(nombreSemaforo,nombreTabla);
	sem_t semaforo;
	sem_t semaforo2;
	sem_init(&semaforo,0,1);
	sem_init(&semaforo2,0,1);
	semaforoTabla* semaforoNuevo = malloc(sizeof(semaforoTabla));
	semaforoNuevo->nombreTabla =nombreSemaforo;
	semaforoNuevo->semaforoTabla=semaforo;
	semaforoNuevo->semaforoCompactacion=semaforo2;
	return semaforoNuevo;
}

void* borrarSemaforo(void* elemento){

	free(((semaforoTabla*)elemento)->nombreTabla);
	sem_destroy(&((semaforoTabla*)elemento)->semaforoTabla);
	sem_destroy(&((semaforoTabla*)elemento)->semaforoCompactacion);
	free(((semaforoTabla*)elemento));
}

//
//t_list* listarTablasExistentes() {
//	DIR *d;
//	struct dirent *dir;
//	char *directorioTablas= DirectorioDeTabla("");
//	d = opendir(directorioTablas);
//	t_list *listaDeTablas = list_create();
//
//	if (d != NULL)
//		  {
//			free(directorioTablas);
//			while ((dir = readdir(d)) != NULL) {
//				if (!string_contains(dir->d_name,"."))
//				{
//
//				char* nombreTabla = malloc(sizeof(strlen(dir->d_name)+1));
//				strcpy(nombreTabla,dir->d_name);
//				list_add(listaDeTablas,nombreTabla);
//
//				}
//
//			}
//		closedir(dir);
//		closedir(d);
//		return listaDeTablas;
//		  } else return listaDeTablas;
//}
//
//







