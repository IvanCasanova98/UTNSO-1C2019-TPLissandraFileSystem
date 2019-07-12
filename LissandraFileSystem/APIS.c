

#include "APIS.h"

void APIdrop(t_paquete_drop* paquete_drop){ //ml 20bytes

	char nombreTablaMayuscula [strlen(paquete_drop->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_drop->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

	if(existeTabla(nombreTablaMayuscula))
	{
		verificarSemaforoCompactacion(nombreTablaMayuscula);
		verificarSemaforoMemTable();
		sem_wait(&SemaforoMemtable);
		RemoverDeLaMemtable(nombreTablaMayuscula); //0 ml
		RemoverParticionesDeTablaYSusBloques(nombreTablaMayuscula); //0 ml
		RemoverTemporalesDeTablaYSusBloques(nombreTablaMayuscula); //8 bytes ml
		RemoverMetadataDeTabla(nombreTablaMayuscula);
		RemoverCarpetaVaciaDeTabla(nombreTablaMayuscula); //ESTA FUNCION SIRVE SOLO SI LA CARPETA ESTA VACIA.
		sem_post(&SemaforoMemtable);
		LogearDropCorrecto(nombreTablaMayuscula);


	} else
	{
		LogearDropFallido(nombreTablaMayuscula);
	}

}



void APIcreate(t_paquete_create* paquete_create){ //0 memory leak
	char nombreTablaMayuscula [strlen(paquete_create->nombre_tabla)+1];

	strcpy(nombreTablaMayuscula,paquete_create->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);
	if(!existeTabla(nombreTablaMayuscula)){
		crearTabla(nombreTablaMayuscula);
		crearMetadataConfig(nombreTablaMayuscula,paquete_create->metadata.consistencia,paquete_create->metadata.particiones,paquete_create->metadata.tiempo_de_compactacion);
		//crearParticiones(nombreTablaMayuscula,paquete_create->metadata.particiones);
		int particiones=paquete_create->metadata.particiones;
		int particionesCargadas=0;
		for(int particionesCargadas=0;particionesCargadas<paquete_create->metadata.particiones;particionesCargadas++)
		crearParticion(nombreTablaMayuscula,particionesCargadas);




		char* nombreTabla= malloc(strlen (nombreTablaMayuscula)+1);
		strcpy(nombreTabla,nombreTablaMayuscula);
		crearHiloCompactacion(nombreTabla);

		liberarPaqueteCreate(paquete_create);


	} else {
		liberarPaqueteCreate(paquete_create);
		LaTablaYaExiste(nombreTablaMayuscula);
	}

}


void APIinsert(t_paquete_insert* paquete_insert){ // 0 memory leak

	char* nombreTablaMayuscula=malloc(strlen(paquete_insert->nombre_tabla)+1);

	strcpy(nombreTablaMayuscula,paquete_insert->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);
	if(existeTabla(nombreTablaMayuscula)){
		verificarSemaforoMemTable();
		sem_wait(&SemaforoMemtable);
		t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
		agregarTabla(paquete_insert);
		LogearInsert(paquete_insert->timestamp,paquete_insert->valor_key,paquete_insert->value,nombreTablaMayuscula);
		liberarPaqueteInsert(paquete_insert);
		imprimirListaTablas(memTable);
		sem_post(&SemaforoMemtable);
		free(metadataDeTabla);
		free(nombreTablaMayuscula);
	} else{
		LaTablaNoExiste(paquete_insert->timestamp,paquete_insert->valor_key,paquete_insert->value,nombreTablaMayuscula);
		liberarPaqueteInsert(paquete_insert);
		free(nombreTablaMayuscula);
	}


}

char* APIselect(t_paquete_select* paquete_select){ // bastante ml revisar


	char* nombreTablaMayuscula=malloc(strlen(paquete_select->nombre_tabla)+1);
	strcpy(nombreTablaMayuscula,paquete_select->nombre_tabla);


	string_to_upper(nombreTablaMayuscula);

		if(existeTabla(nombreTablaMayuscula)){
			verificarSemaforoTabla(nombreTablaMayuscula);
			t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
			int particionKey;
			particionKey =	particionDeKey(paquete_select->valor_key,metadataDeTabla->particiones);
			t_list* RegistrosEncontrados=list_create();

			void* RegistroParticion 		  = buscarEnParticion(nombreTablaMayuscula,particionKey,paquete_select->valor_key);
			void* RegistroMemTable  		  = buscarMemTable(nombreTablaMayuscula,paquete_select->valor_key); // 8 bytes ml
			void* RegistroTemporal  		  = buscarEnTemporales(nombreTablaMayuscula,paquete_select->valor_key);
			void* RegistroTemporalCompactando = buscarEnTemporalesCompactando(nombreTablaMayuscula,paquete_select->valor_key);

			if(RegistroMemTable!=NULL){
			list_add(RegistrosEncontrados,RegistroMemTable);
			}
			if(RegistroParticion!=NULL){
			list_add(RegistrosEncontrados,RegistroParticion);

			}
			if(RegistroTemporal!=NULL){
			list_add(RegistrosEncontrados,RegistroTemporal);

			}
			if(RegistroTemporalCompactando!=NULL){
			list_add(RegistrosEncontrados,RegistroMemTable);
			}

			if(!list_is_empty(RegistrosEncontrados)){
			char* valueEncontrado = elegirMayorTimeStamp(RegistrosEncontrados);
			list_clean_and_destroy_elements(RegistrosEncontrados,liberarRegistro);
			list_destroy(RegistrosEncontrados);

			valueExiste(nombreTablaMayuscula,paquete_select->valor_key,valueEncontrado);

			liberarPaqueteSelect(paquete_select);

			free(metadataDeTabla);
			free(nombreTablaMayuscula);

			return valueEncontrado;


//			return NULL;
			}else{
				valueNoExiste(paquete_select->nombre_tabla,paquete_select->valor_key);

				list_destroy(RegistrosEncontrados);
				free(nombreTablaMayuscula);
				free(metadataDeTabla);
				liberarPaqueteSelect(paquete_select);
				return NULL;
		}

		}else{
		LaTablaNoExisteSelect(nombreTablaMayuscula);
		free(nombreTablaMayuscula);
		liberarPaqueteSelect(paquete_select);
		return NULL;}
}


t_dictionary* APIdescribe (t_paquete_describe* paquete_describe){

	char nombreTablaMayuscula [strlen(paquete_describe->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_describe->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

	if(existeTabla(nombreTablaMayuscula)){
		t_metadata* metadataDeTabla = obtenerMetadataTabla(nombreTablaMayuscula);
		t_dictionary* metadataParticularSolicitada = dictionary_create();

		dictionary_put(metadataParticularSolicitada,nombreTablaMayuscula,metadataDeTabla);
//		free(metadataDeTabla);

		logearDescribeTablaEnParticular(nombreTablaMayuscula);
		return metadataParticularSolicitada;

	} else {
		logearDescribeTablaInexistente(nombreTablaMayuscula);
		return NULL;
	}

}

t_dictionary* APIdescribeTodasLasTablas(){
//	void* eliminarNombreTablaHallada(void *elemento){
//		free((char*) elemento);
//	}
	  DIR *d;
	  struct dirent *dir;
	  char *directorioTablas= DirectorioDeTabla("");
	  d = opendir(directorioTablas);
	  t_dictionary *diccionarioDeTablas = dictionary_create();
	  if (d != NULL)
	  {
		free(directorioTablas);
	    while ((dir = readdir(d)) != NULL) {
	    	if (!string_contains(dir->d_name,".")){
				char* nombreTabla = malloc(sizeof(dir->d_name)+1);
				strcpy(nombreTabla,dir->d_name);
				t_metadata* metadataDeTabla = obtenerMetadataTabla(nombreTabla);

				dictionary_put(diccionarioDeTablas,nombreTabla,metadataDeTabla);
				free(nombreTabla);
	    	}

	    }

	    closedir(dir);
	    closedir(d);

	    logearDescribeTodasLasTablas();
	    return diccionarioDeTablas;
	  } else return diccionarioDeTablas;


}



void imprimirListaMetadatas(t_dictionary * metadatas){
	dictionary_iterator(metadatas,mostrarMetadataTablas);
}


void mostrarMetadataTablas(char*nombreTabla,void* elemento){

	printf("%s%s%s  ",CYAN,nombreTabla,MAGENTA);
	imprimirMetadata((t_metadata*)elemento);
	printf("%s\n",NORMAL_COLOR);


}






//void listarTablas(){ //0 ml
//		DIR *d;
//	  struct dirent *dir;
//	  char *directorioTablas= DirectorioDeTabla("");
//	  d = opendir(directorioTablas);
//	  if (d != NULL)
//	  {
//		while ((dir = readdir(d)) != NULL) {
//			if (!string_contains(dir->d_name,".")) {
//
//
//			}
//		}
//		closedir(d);
//	  }
//}

void imprimirMetadata(t_metadata* metadataDeTablaPedida){
	printf("%s ",pasarAConsistenciaChar(metadataDeTablaPedida->consistencia));
	printf(" %d ",metadataDeTablaPedida->particiones);
	printf(" %d\n",metadataDeTablaPedida->tiempo_de_compactacion);
	printf("%s",NORMAL_COLOR);
}



t_list* listarTablasExistentes() {
  DIR *d;
  struct dirent *dir;
  char *directorioTablas= DirectorioDeTabla("");
  d = opendir(directorioTablas);
  t_list *listaDeTablas = list_create();
  if (d != NULL)
  {
	free(directorioTablas);
    while ((dir = readdir(d)) != NULL) {
    	if (!string_contains(dir->d_name,".")){

    	char* nombreTabla = malloc(sizeof(strlen(dir->d_name)+1));
    	strcpy(nombreTabla,dir->d_name);
    	list_add(listaDeTablas,nombreTabla);
//    	free(nombreTabla);
    	}

    }
    closedir(dir);
    closedir(d);
//    list_destroy(listaDeTablas);
    return listaDeTablas;
  } else return listaDeTablas;

}


char* elegirMayorTimeStamp(t_list* RegistrosEncontrados){
	list_sort(RegistrosEncontrados,mayorTimeStamp);
	t_registro* registroConMayorTimeStamp = (t_registro*)list_remove(RegistrosEncontrados,0);

	char * value =malloc(strlen(registroConMayorTimeStamp->value)+1);
	strcpy(value,registroConMayorTimeStamp->value);
	liberarRegistro(registroConMayorTimeStamp);
	return value;
}

t_registro* crearRegistro (char*value,uint16_t key,long long timestamp){
	t_registro* registroNuevo=malloc(sizeof(t_registro));
	registroNuevo->value=malloc(strlen(value)+2);
	strcpy(registroNuevo->value,value);
	registroNuevo->key=key;
	registroNuevo->timestamp=timestamp;
	return registroNuevo;
}

int particionDeKey(int key,int particiones){

	return key%particiones;

}

t_metadata* obtenerMetadataTabla (char* nombreTabla){ //0 ml

	char* ubicacionMetaData = DirectorioDeMetadataTabla(nombreTabla);
	t_config* config = config_create(ubicacionMetaData);
	char* consistencia = config_get_string_value(config,"CONSISTENCY");
	int particiones = atoi(config_get_string_value(config,"PARTITIONS"));
	int compactacion = atoi(config_get_string_value(config,"COMPACTION_TIME"));
	t_metadata* metadataObtenida=crearMetadata(pasarAConsistenciaINT(consistencia),particiones,compactacion);
	free(ubicacionMetaData);
	config_destroy(config);
	return metadataObtenida;

}


int existeTabla(char* TablaBuscada){ //0 ml

	char* directorioTablas = DirectorioDeTabla(TablaBuscada);

	DIR* dir = opendir(directorioTablas);
	if (dir)
	{
	    closedir(dir);
	    free(directorioTablas);
	    return 1;
	}
	else if (ENOENT == errno)
	{
	    free(directorioTablas);
	    return 0;
	}
	else
	{

		printf("Fallo desconocido :(\n");

		free(directorioTablas);
	    return 0;
	}
}





t_metadata* crearMetadata(consistency consistencia, int particiones,int tiempo_compactacion){ //0 ml
	t_metadata* metadata = malloc(sizeof(int)*3);

	metadata->consistencia=consistencia;
	metadata->particiones=particiones;
	metadata->tiempo_de_compactacion=tiempo_compactacion;

	return metadata;

}
bool validarConsistencia(char* consistencia){ //0 ml

	if (strcmp(consistencia, "SC")==0) {
		return true;
	}
	else if (strcmp(consistencia, "SHC")==0) {
		return true;
	}
	else if (strcmp(consistencia, "EC")==0) {
		return true;

	}
	return false;

}

//PASAR CONSISTENCIAS
int pasarAConsistenciaINT(char* consistencia){ //0 ml

	if (strcmp(consistencia, "SC")==0) {
		return 0;
	}
	else if (strcmp(consistencia, "SHC")==0) {
		return 1;
	}
	else if (strcmp(consistencia, "EC")==0) {
		return 2;

	}
	return 0;

}

char* pasarAConsistenciaChar(int consistencia){ //0 ml

	if (consistencia==0) {
		return "SC";
	}
	else if (consistencia==1) {
		return "SHC";
	}
	else if (consistencia==2) {
		return "EC";
	}
	return "SC";

}

void logLaTablaYaExiste(){ //0 ml
	t_log* logger = iniciar_logger();
	log_info(logger, "ERROR LA TABLA YA EXISTE");
    log_destroy(logger);
}

void crearTabla(char* nombreTabla){ //0 ml
	char* DirectorioTablas = DirectorioDeTabla(nombreTabla);
	mkdir(DirectorioTablas,0777);
	free(DirectorioTablas);
}


char* DirectorioDeTabla(char* tabla){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(tabla) +strlen("Tables/") +1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,tabla);
	config_destroy(config);
	return directorioTablas;
}


char* DirectorioDeMetadataTabla(char* tabla){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(tabla) +strlen("Tables/") + strlen("/Metadata.config") +1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,tabla);
	strcat(directorioTablas,"/Metadata.config");
	//free(Montaje);
	config_destroy(config);
	return directorioTablas;

}
char* DirectorioDeMetadata(){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioMetadata = malloc(strlen(Montaje)  +strlen("Metadata/Metadata.bin")  +1);
	strcpy(directorioMetadata,Montaje);
	strcat(directorioMetadata,"Metadata/Metadata.bin");
	config_destroy(config);
	return directorioMetadata;

}
char* DirectorioDeParticion(char* nombretabla,int numeroParticion){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char buffer [3];
	sprintf(buffer,"/%d",numeroParticion);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(".bin") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".bin");
	config_destroy(config);
	return directorioTablas;


}

int existe_temporal(char* path){ //0 ml
	struct stat buffer;
	return(stat(path,&buffer)==0);
}





void RemoverTemporalesDeTablaYSusBloques(char* nombretabla){
	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);


	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");

	char* directorioAux= malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(int)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	int i =1;
	strcpy(directorioAux,directorioTablas);
	strcat(directorioAux,string_itoa(i));
	strcat(directorioAux,".tmp");
	while(existe_temporal(directorioAux)){
		t_config* directorioTemporal= config_create(directorioAux);
		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorioTemporal,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		remove(directorioAux);
		config_destroy(directorioTemporal); //AGREGADO

		i++;
		strcpy(directorioAux,directorioTablas);
		strcat(directorioAux,string_itoa(i));
		strcat(directorioAux,".tmp");
	}

	config_destroy(config); //AGREGADO
	free(directorioTablas); //AGREGADO
	free(directorioAux);
	free(NombreTmp); //AGREGADO

}



char* DirectorioDeTemporalNuevo(char* nombretabla){ //0 ml

	t_config* config = config_create("Lissandra.config");
	char* nombreDadoVueltao=string_reverse(nombretabla);
	char* NombreTmp = string_substring_until(nombreDadoVueltao, 1);

	char buffer [3];
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char* directorioAux= malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	int i =1;
	strcpy(directorioAux,directorioTablas);
	sprintf(buffer,"%d",i);
	strcat(directorioAux,buffer);
	strcat(directorioAux,".tmp");
	while(existe_temporal(directorioAux)){
		i++;
		strcpy(directorioAux,directorioTablas);
		sprintf(buffer,"%d",i);
		strcat(directorioAux,buffer);
		strcat(directorioAux,".tmp");
	}

	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".tmp");
	config_destroy(config);
	free(directorioAux);
	free(nombreDadoVueltao);
	free(NombreTmp);
	return directorioTablas;


}

char* DirectorioDeTemporal(char* nombretabla,int nroTemporal){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* nombreDadoVuelta=string_reverse(nombretabla);
	char* NombreTmp = string_substring_until(nombreDadoVuelta, 1);

	char buffer [3];
	sprintf(buffer,"%d",nroTemporal);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".tmp");
	config_destroy(config);
	free(nombreDadoVuelta);
	free(NombreTmp);
	return directorioTablas;


}

char* DirectorioDeTemporalCompactacion(char* nombretabla,int nroTemporal){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* nombreDadoVuelta=string_reverse(nombretabla);
	char* NombreTmp = string_substring_until(nombreDadoVuelta, 1);

	char buffer [3];
	sprintf(buffer,"%d",nroTemporal);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmpc") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".tmpc");
	config_destroy(config);
	free(NombreTmp);
	free(nombreDadoVuelta);
	return directorioTablas;


}



char* DirectorioDeBloque(int numeroBloque){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* buffer=malloc(sizeof(int));
	char* numeroBloqueChar =string_itoa(numeroBloque);
	strcpy(buffer,numeroBloqueChar);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioBloque = malloc(strlen(Montaje) + strlen(".bin") +strlen("Bloques/") +sizeof(buffer)+6);
	strcpy(directorioBloque,Montaje);
	strcat(directorioBloque,"Bloques/");
	strcat(directorioBloque,buffer);
	strcat(directorioBloque,".bin");
	config_destroy(config);
	free(buffer);
	free(numeroBloqueChar);
	return directorioBloque;


}

char* DirectorioBitMap(){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioMetadata = malloc(strlen(Montaje)  +strlen("Metadata/bitmap.bin")  +1);
	strcpy(directorioMetadata,Montaje);
	strcat(directorioMetadata,"Metadata/bitmap.bin");
	config_destroy(config);
	return directorioMetadata;

}

void crearMetadataConfig(char*nombreTablaMayuscula, consistency consistencia, int particiones,int tiempo_compactacion){ //0 ml
	char *directorioTablaNueva = DirectorioDeMetadataTabla(nombreTablaMayuscula);
	FILE *fp=NULL;
	fp=fopen(directorioTablaNueva,"a");
	fprintf(fp,"CONSISTENCY=%s\nPARTITIONS=%d\nCOMPACTION_TIME=%d",pasarAConsistenciaChar(consistencia),particiones,tiempo_compactacion);
	fclose(fp);
	free(directorioTablaNueva);
}



void crearParticiones(char*nombreTabla ,int nroParticiones){ // no anda pero la dejo x las dudas
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	while(blockNum%8!=0){
			blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	FILE *particionBin;


	int i=0;
	while(nroParticiones){

	if(!bitarray_test_bit(bitmap,i)){
		particionBin=fopen(DirectorioDeParticion(nombreTabla,nroParticiones),"w");
		fprintf(particionBin,"SIZE=0\nBLOCKS=[%d]",i);
		nroParticiones--;
		bitarray_set_bit(bitmap,i);
		fclose(particionBin);
	}
	i++;
	}
	free(directorioMetadata);
	free(directorioBitmap);
	fclose(particionBin);
	config_destroy(config);

}

void crearParticion(char*nombreTabla ,int nroParticion){ //0 ml
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	while(blockNum%8!=0){
			blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	FILE *particionBin;


	int i=0;
	while(i<blockNum){

	if(!bitarray_test_bit(bitmap,i)){
		char * directorioParticion=DirectorioDeParticion(nombreTabla,nroParticion);
		particionBin=fopen(directorioParticion,"w");
		char* Script= malloc(strlen("SIZE=0\nBLOCKS=[%d]")+1);
		strcpy(Script,"SIZE=0\nBLOCKS=[%d]");
		fprintf(particionBin,"SIZE=0\nBLOCKS=[%d]",i);
		fflush(particionBin);
		bitarray_set_bit(bitmap,i);
		crearBloque(i);
		free(Script);
		free(directorioParticion);
		//ActualizarBitmap();
		break;//bitarray_set_bit(bitmap, i);
	}
	i++;
	}
	free(directorioMetadata);
	free(directorioBitmap);
	fclose(particionBin);
	//free(particionBin);
	config_destroy(config);


}
//free(nombreTablaMayus);
void RemoverCarpetaVaciaDeTabla(char *nombreTabla){
	char *directorioDeTablaABorrar= DirectorioDeTabla(nombreTabla);
	remove(directorioDeTablaABorrar);
	free(directorioDeTablaABorrar);
}

void RemoverMetadataDeTabla(char *nombreTabla){
	char *directorioDeTablaABorrar= DirectorioDeMetadataTabla(nombreTabla);
	remove(directorioDeTablaABorrar);
	free(directorioDeTablaABorrar);

}

void RemoverDeLaMemtable(char *nombreTabla){
	if(memTable != NULL)
	{
		if(dictionary_has_key(memTable,nombreTabla))
		{
		dictionary_remove_and_destroy(memTable,nombreTabla,liberarNodo);
		}

		if(dictionary_is_empty(memTable)){
			dictionary_destroy(memTable);
			memTable=NULL;
		}

	}
}

void RemoverParticionDeTablaEspecificaYSusBloques(char* nombreTabla,int nroParticionABorrar){
	t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTabla);
	int particiones = metadataDeTabla->particiones;
	free(metadataDeTabla);

		char *directorioDeTablaABorrar= DirectorioDeParticion(nombreTabla,nroParticionABorrar);
		t_config* directorio = config_create(directorioDeTablaABorrar);

		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorio,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		//ESTA FUNCION REMUEVE CADA BLOQUE ASIGNADO A ESA PARTICION.

		remove(directorioDeTablaABorrar); //ESTE REMOVE REMUEVE LA PARTICION.
		for(int i=0;arrayBloques[i];i++)
			free(arrayBloques[i]);
			free(arrayBloques);

		config_destroy(directorio);
		free(directorioDeTablaABorrar);

}

void RemoverParticionesDeTablaYSusBloques(char* nombreTabla){
	t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTabla);
	int particiones = metadataDeTabla->particiones;
	free(metadataDeTabla);

	for(int i=0;i<particiones;i++){

		char *directorioDeTablaABorrar= DirectorioDeParticion(nombreTabla,i);
		t_config* directorio = config_create(directorioDeTablaABorrar);

		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorio,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		//ESTA FUNCION REMUEVE CADA BLOQUE ASIGNADO A ESA PARTICION.

		remove(directorioDeTablaABorrar); //ESTE REMOVE REMUEVE LA PARTICION.
		for(int i=0;arrayBloques[i];i++)
			free(arrayBloques[i]);
			free(arrayBloques);

		config_destroy(directorio);
		free(directorioDeTablaABorrar);
	}
}

void removerBloque(char* nroBloque){
	char *directorioDeBloque = DirectorioDeBloque(atoi(nroBloque));
	bitarray_clean_bit(bitmap,atoi(nroBloque));
	remove(directorioDeBloque);
	free(directorioDeBloque);
}


t_registro* buscarEnParticion(char* nombreTabla, int nroParticion,int key){
		char* directorioParticion = DirectorioDeParticion(nombreTabla,nroParticion);
		t_config* config = config_create(directorioParticion);
		char** arrayBloques = config_get_array_value(config, "BLOCKS");
		int i=0;
		char* registrosCompletos=string_new();
		while(arrayBloques[i]){

			int nroBloque = atoi(arrayBloques[i]);
			char* registrosDeBloque= ObtenerContenidoBloque(nroBloque);
			if(string_is_empty(registrosDeBloque)){
				free(registrosDeBloque);
				free(registrosCompletos);
				for(int i=0;arrayBloques[i];i++)
				free(arrayBloques[i]);
				free(arrayBloques);
				free(directorioParticion);
				config_destroy(config);
				return NULL;
			}
			string_append(&registrosCompletos,registrosDeBloque);
			free(registrosDeBloque);
			if(arrayBloques[i+1]==NULL){
				config_destroy(config);
				for(int i=0;arrayBloques[i];i++)
				free(arrayBloques[i]);
				free(arrayBloques);
				free(directorioParticion);
				return buscarKey(registrosCompletos,key);
			}else{
				char** registrosSeparados=string_split(registrosCompletos,"\n");
				int registrosRecorridos=0;
				while (registrosSeparados[1]==NULL){
					if(arrayBloques[i+1]==NULL){

						for(int i=0;arrayBloques[i];i++)
						free(arrayBloques[i]);
						free(arrayBloques);

						for(int i=0;registrosSeparados[i];i++)
						free(registrosSeparados[i]);
						free(registrosSeparados);

						free(directorioParticion);
						//free(registrosDeBloque);
						config_destroy(config);
						return buscarKey(registrosCompletos,key);
					}else{

					i++;
					nroBloque = atoi(arrayBloques[i]);
					registrosDeBloque= ObtenerContenidoBloque(nroBloque);
					string_append(&registrosCompletos,registrosDeBloque);
					free(registrosDeBloque);

					for(int i=0;registrosSeparados[i];i++)
					free(registrosSeparados[i]);
					free(registrosSeparados);
					registrosSeparados=string_split(registrosCompletos,"\n");
					}
				}

				char** RegistroActual;
				while(registrosSeparados[registrosRecorridos+1]!=NULL){
				char** RegistroActual =string_split(registrosSeparados[registrosRecorridos],";");

				if(atoi(RegistroActual[1])==key){
					t_registro* registroEncontrado=crearRegistro(RegistroActual[2],atoi(RegistroActual[1]),atoll(RegistroActual[0]));
					for(int i=0;arrayBloques[i];i++)
					free(arrayBloques[i]);
					free(arrayBloques);

					for(int i=0;registrosSeparados[i];i++)
					free(registrosSeparados[i]);
					free(registrosSeparados);

					for(int i=0;RegistroActual[i];i++)
					free(RegistroActual[i]);
					free(RegistroActual);

	//				free(registrosCompletos);
					free(directorioParticion);
					config_destroy(config);
					return registroEncontrado;
				}
				registrosRecorridos++;
				free(registrosCompletos);
				registrosCompletos=string_new();
				registrosCompletos=registrosSeparados[registrosRecorridos];

			}

			i++;



		}

	}
}



t_registro* buscarKey(char* registrosDeBloque,int key){

	char** registrosSeparados=string_split(registrosDeBloque,"\n");
	int i=0;
	char** RegistroActual;
	while(registrosSeparados[i]!=NULL){
	char** RegistroActual =string_split(registrosSeparados[i],";");
	if(registrosSeparados[0]==NULL){

		free(registrosDeBloque);

		for(int cerrar=0;registrosSeparados[cerrar];cerrar++)
		free(registrosSeparados[cerrar]);
		free(registrosSeparados);

		for(int cerrar=0;RegistroActual[cerrar];cerrar++)
		free(RegistroActual[cerrar]);
		free(RegistroActual);
		return NULL;
	}
	if(atoi(RegistroActual[1])==key){
		t_registro* registroEncontrado=crearRegistro(RegistroActual[2],atoi(RegistroActual[1]),atoll(RegistroActual[0]));
		free(registrosDeBloque);

		for(int cerrar=0;registrosSeparados[cerrar];cerrar++)
		free(registrosSeparados[cerrar]);
		free(registrosSeparados);


		for(int cerrar=0;RegistroActual[cerrar];cerrar++)
		free(RegistroActual[cerrar]);
		free(RegistroActual);

		return registroEncontrado;
	}
	i++;
	for(int cerrar=0;RegistroActual[cerrar];cerrar++)
	free(RegistroActual[cerrar]);
	free(RegistroActual);
	}
	free(registrosDeBloque);


	for(int cerrar=0;registrosSeparados[cerrar];cerrar++)
	free(registrosSeparados[cerrar]);

	free(registrosSeparados);

//	for(int cerrar=0;RegistroActual[cerrar];cerrar++)
//	free(RegistroActual[cerrar]);
//
//	free(RegistroActual);
	return NULL;

	//printf("hoasd");



//	}
//	i++;
//
//	}


}


void liberarPaqueteInsert(t_paquete_insert* paquete_insert){
	free(paquete_insert->nombre_tabla);
	free(paquete_insert->value);
	free(paquete_insert);
}
void liberarPaqueteSelect(t_paquete_select* paquete_select){
	free(paquete_select->nombre_tabla);
	free(paquete_select);
}

void liberarPaqueteCreate(t_paquete_create* paquete_create){
	free(paquete_create->nombre_tabla);
	free(paquete_create);
}

void liberarPaqueteDrop(t_paquete_drop* paquete_drop){
	free(paquete_drop->nombre_tabla);
	free(paquete_drop);
}


t_registro* buscarEnTemporales(char* nombreTabla,int key){

	int temporales=1;
	char* directorioTemporal= DirectorioDeTemporal(nombreTabla,temporales);
	char* registrosCompletos=string_new();
	while(existe_temporal(directorioTemporal)){
		int i=0;
		t_config* config = config_create(directorioTemporal);
		char** arrayBloques = config_get_array_value(config, "BLOCKS");
		while(arrayBloques[i]){
			int nroBloque = atoi(arrayBloques[i]);
			char* registrosDeBloque= ObtenerContenidoBloque(nroBloque);
			string_append(&registrosCompletos,registrosDeBloque);
			free(registrosDeBloque);
			i++;
		}
	config_destroy(config);
	string_iterate_lines(arrayBloques,free);
	free(arrayBloques);
	free(directorioTemporal);
	temporales++;
	directorioTemporal= DirectorioDeTemporal(nombreTabla,temporales);
	}
	free(directorioTemporal);
	if(temporales==1) { free(registrosCompletos);return NULL;}


	return buscarRegistroTemporalMasReciente(registrosCompletos,key);
}

t_registro* buscarEnTemporalesCompactando(char* nombreTabla,int key){


	int temporales=1;
	char* directorioTemporalcompactando= DirectorioDeTemporalCompactacion(nombreTabla,temporales);
	char* registrosCompletos=string_new();
	while(existe_temporal(directorioTemporalcompactando)){
		int i=0;
		t_config* config = config_create(directorioTemporalcompactando);
		char** arrayBloques = config_get_array_value(config, "BLOCKS");
		while(arrayBloques[i]){
			int nroBloque = atoi(arrayBloques[i]);
			char* registrosDeBloque= ObtenerContenidoBloque(nroBloque);
			string_append(&registrosCompletos,registrosDeBloque);
			free(registrosDeBloque);
			i++;
		}
	config_destroy(config);
	string_iterate_lines(arrayBloques,free);
	free(arrayBloques);
	free(directorioTemporalcompactando);
	temporales++;
	directorioTemporalcompactando= DirectorioDeTemporalCompactacion(nombreTabla,temporales);
	}
	free(directorioTemporalcompactando);
	if(temporales==1) {free(registrosCompletos);return NULL;}


	return buscarRegistroTemporalMasReciente(registrosCompletos,key);
}


t_registro* buscarRegistroTemporalMasReciente(char* todosLosRegistrosTemporales,int key){
	t_list* listaDeRegistros=list_create();
	int registrosRecorridos=0;
	char**arrayRegistros= string_split(todosLosRegistrosTemporales,"\n");
	while(arrayRegistros[registrosRecorridos]!=NULL){
		char** RegistroActual =string_split(arrayRegistros[registrosRecorridos],";");
		list_add(listaDeRegistros,crearRegistro(RegistroActual[2],atoi(RegistroActual[1]),atoll(RegistroActual[0])));
		registrosRecorridos++;
		string_iterate_lines(RegistroActual,free);
		free(RegistroActual);
		}
		bool _filtradoKey(void* elemento){
			return filtrarPorKeyRegistro(elemento,key);
		}
		if(!listaVacia(listaDeRegistros)){
		t_list* listaRegistrosKey = list_filter(listaDeRegistros,_filtradoKey);
		if(!listaVacia(listaRegistrosKey)){
		list_sort(listaRegistrosKey,mayorTimeStampRegistro);

		t_registro* AuxregistroEncontrado = list_get(listaRegistrosKey, 0); // 0 es el primero
		t_registro* registroEncontrado = crearRegistro(AuxregistroEncontrado->value,AuxregistroEncontrado->key,AuxregistroEncontrado->timestamp);


		list_clean_and_destroy_elements(listaDeRegistros,liberarRegistro);

		string_iterate_lines(arrayRegistros,free);

		free(arrayRegistros);

		free(todosLosRegistrosTemporales);



		list_destroy(listaRegistrosKey);

		list_destroy(listaDeRegistros);

		return registroEncontrado;
		}
		list_destroy(listaRegistrosKey);
		}
		string_iterate_lines(arrayRegistros,free);
		free(arrayRegistros);
		list_clean_and_destroy_elements(listaDeRegistros,liberarBloque);
		list_destroy(listaDeRegistros);
		free(todosLosRegistrosTemporales);
		//free(arrayRegistros);
		return NULL;
}


bool filtrarPorKeyRegistro(void* elemento,int key){
	return ((t_registro*) elemento)->key == key;
}



bool mayorTimeStampRegistro(void*elemento1,void*elemento2){
	return ((t_registro*) elemento1)->timestamp > ((t_registro*) elemento2)->timestamp;
}


void liberarRegistro(void* elemento){
	free(((t_registro*)elemento)->value);
	free(((t_registro*)elemento));
}

void listarTablas(){ //0 ml
		DIR *d;
	  struct dirent *dir;
	  char *directorioTablas= DirectorioDeTabla("");
	  d = opendir(directorioTablas);
	  if (d != NULL)
	  {
		while ((dir = readdir(d)) != NULL) {
			if (!string_contains(dir->d_name,".")) {

				printf("%s%s%s  ",CYAN,dir->d_name,NORMAL_COLOR);
			}
		}
		closedir(d);
	  }
}


void verificarSemaforoTabla(char* nombreTabla){
	int valorSemaforo;
	bool _buscarSemaforo(void* elemento){
					return buscarSemaforo(elemento,nombreTabla);
		}
	semaforoTabla* Tablasemaforo = list_find(ListaSem,_buscarSemaforo);
	sem_getvalue(&(Tablasemaforo->semaforoTabla),&valorSemaforo);

	if(valorSemaforo==0){
		sem_wait(&(Tablasemaforo->semaforoTabla));
		sem_post(&(Tablasemaforo->semaforoTabla));

	}


}

void verificarSemaforoCompactacion(char* nombreTabla){
	int valorSemaforo;
	bool _buscarSemaforo(void* elemento){
					return buscarSemaforo(elemento,nombreTabla);
	}
	semaforoTabla* Tablasemaforo = list_find(ListaSem,_buscarSemaforo);

	sem_getvalue(&(Tablasemaforo->semaforoCompactacion),&valorSemaforo);

	if(valorSemaforo==0){
		sem_wait(&(Tablasemaforo->semaforoCompactacion));
		sem_post(&(Tablasemaforo->semaforoCompactacion));

	}


}

void verificarSemaforoMemTable(){
	int valorSemaforo;


	sem_getvalue(&SemaforoMemtable,&valorSemaforo);

	if(valorSemaforo==0){
		sem_wait(&SemaforoMemtable);
		sem_post(&SemaforoMemtable);

	}


}



//void notificarCambioRetardo(){
//
//	char buffer [2*sizeof(struct inotify_event )+24];
//	int file_descriptor = inotify_init();
//
//	int watch_descriptor = inotify_add_watch(file_descriptor, "Lissandra.config", IN_MODIFY | IN_CREATE | IN_DELETE);
//	int length = read(file_descriptor, buffer, sizeof(struct inotify_event )+24);
//	int offset = 0;
//	while (offset < length) {
//	struct inotify_event *event = (struct inotify_event *) &buffer[offset];
//	if (event->len) {
//
//	}
//	}
//}



