

#include "APIS.h"

void APIdrop(t_paquete_drop* paquete_drop){

	char nombreTablaMayuscula [strlen(paquete_drop->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_drop->nombre_tabla);

	LogearDrop(nombreTablaMayuscula);

	//BORRAR DE LA MEMTABLE.

	if(existeTabla(nombreTablaMayuscula)){
		if(memTable != NULL)
		{
			if(dictionary_has_key(memTable,nombreTablaMayuscula))
			{
			dictionary_remove_and_destroy(memTable,nombreTablaMayuscula,liberarNodo);
			printf("Se eliminaron los registros de %s de la memTable.\n",nombreTablaMayuscula);
			} else {printf("No se hallaron registros en la memTable de %s\n",nombreTablaMayuscula);}

		}

		char *directorioDeTablaABorrar = DirectorioDeTabla(nombreTablaMayuscula);

		t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
		int particiones = metadataDeTabla->particiones;
		free(metadataDeTabla);
//		rmdir(directorioDeTablaABorrar); //BORRA LA CARPETA SI ESTA VACIA.

//		printf("el directorio es: %s", directorioDeTablaABorrar);
//		IMPRIME /home/utnso/LISSANDRA_FS/Tables/TABLA_B


		//---- ESTO FUNCIONA BORRA 1.bin
//		char buffer [3];
//		sprintf(buffer,"/%d",1);
//		strcat(directorioDeTablaABorrar,buffer);
//		strcat(directorioDeTablaABorrar,".bin");
//		remove(directorioDeTablaABorrar);
		//---- ESTO FUNCIONA BORRA 1.bin

		char buffer [3];
//		char *nroParticion;
//		for(int i=1;i<=particiones;i++){
///

			sprintf(buffer,"/%c",string_itoa(1));
			strcat(directorioDeTablaABorrar,buffer);
			strcat(directorioDeTablaABorrar,".bin");
			printf(directorioDeTablaABorrar);
			t_config* directorio = config_create(directorioDeTablaABorrar);
//			char **arrayBloques = string_get_string_as_array(config_get_string_value(directorio,"BLOCKS"));
//			string_iterate_lines(arrayBloques,removerBloque);
			//ESTA FUNCION REMUEVE CADA BLOQUE ASIGNADO A ESA PARTICION.

//			free(arrayBloques);
//			remove(directorioDeTablaABorrar);
//			config_destroy(directorio);
//			free(directorioDeTablaABorrar);
			//ESTE REMOVE REMUEVE LA PARTICION.
//			strcpy(directorioDeTablaABorrar,DirectorioDeTabla(nombreTablaMayuscula));
//		}
//=======
//	string_to_upper(nombreTablaMayuscula);
//>>>>>>> 85608229631813e61806435b31f1a8ba35f5d25c
//
//	if(existeTabla(nombreTablaMayuscula))
//	{
//		LogearDropCorrecto(nombreTablaMayuscula);
//
//		RemoverDeLaMemtable(nombreTablaMayuscula);
//		RemoverParticionesDeTablaYSusBloques(nombreTablaMayuscula);
//		RemoverTemporalesDeTablaYSusBloques(nombreTablaMayuscula);
//		RemoverMetadataDeTabla(nombreTablaMayuscula);
//		RemoverCarpetaVaciaDeTabla(nombreTablaMayuscula); //ESTA FUNCION SIRVE SOLO SI LA CARPETA ESTA VACIA.
//
//		printf("%s BORRADA.\n",nombreTablaMayuscula);
//
//	} else
//	{
//		LogearDropFallido(nombreTablaMayuscula);
//		error_show("No existe esa tabla.\n");
//	}
//
//
//}


void APIcreate(t_paquete_create* paquete_create){ //0 memory leak
	char nombreTablaMayuscula [strlen(paquete_create->nombre_tabla)+1];
	string_to_upper(nombreTablaMayuscula);
	strcpy(nombreTablaMayuscula,paquete_create->nombre_tabla);

	if(!existeTabla(nombreTablaMayuscula)){
		crearTabla(nombreTablaMayuscula);
		crearMetadataConfig(nombreTablaMayuscula,paquete_create->metadata.consistencia,paquete_create->metadata.particiones,paquete_create->metadata.tiempo_de_compactacion);
		//crearParticiones(nombreTablaMayuscula,paquete_create->metadata.particiones);
		int particiones=paquete_create->metadata.particiones;
		int particionesCargadas=0;
		for(int particionesCargadas=0;particionesCargadas<paquete_create->metadata.particiones;particionesCargadas++)
		crearParticion(nombreTablaMayuscula,particionesCargadas);

		liberarPaqueteCreate(paquete_create);
	} else if (EEXIST==errno) LaTablaYaExiste(nombreTablaMayuscula);

}


void APIinsert(t_paquete_insert* paquete_insert){ // 0 memory leak
	/*
		 *1. Verificar que la tabla exista en el file system. En caso que no exista, informa el error y continúa su ejecución.
		 *2. Obtener la metadata asociada a dicha tabla.
		 *3. Verificar si existe en memoria una lista de datos a dumpear. De no existir, alocar dicha memoria.
		 *4. El parámetro Timestamp es opcional. En caso que un request no lo provea (por ejemplo insertando un valor desde la consola), se usará el valor actual del Epoch UNIX.
		 *5. Insertar en la memoria temporal del punto anterior una nueva entrada que contenga los datos enviados en la request.

		 * En los request solo se utilizarán las comillas (“”) para enmascarar el Value que se envíe. No se proveerán request con comillas en otros puntos.
	 */

	char* nombreTablaMayuscula=malloc(strlen(paquete_insert->nombre_tabla)+1);

	strcpy(nombreTablaMayuscula,paquete_insert->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);
	if(existeTabla(nombreTablaMayuscula)){
		t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
		agregarTabla(paquete_insert);
		LogearInsert(paquete_insert->timestamp,paquete_insert->valor_key,paquete_insert->value,nombreTablaMayuscula);
		liberarPaqueteInsert(paquete_insert);
		imprimirListaTablas(memTable);
		free(metadataDeTabla);
		free(nombreTablaMayuscula);
	} else{
		LaTablaNoExiste(paquete_insert->timestamp,paquete_insert->valor_key,paquete_insert->value,nombreTablaMayuscula);
		liberarPaqueteInsert(paquete_insert);
		free(nombreTablaMayuscula);
	}


}

char* APIselect(t_paquete_select* paquete_select){ // bastante ml revisar
	/*
	 *1. Verificar que la tabla exista en el file system.
	 *2. Obtener la metadata asociada a dicha tabla.
	 *3. Calcular cual es la partición que contiene dicho KEY.
	 *4. Escanear la partición objetivo, todos los archivos temporales y la memoria temporal de dicha tabla (si existe) buscando la key deseada.
	 *5. Encontradas las entradas para dicha Key, se retorna el valor con el Timestamp más grande.
	 *
	 */


	char* nombreTablaMayuscula=malloc(strlen(paquete_select->nombre_tabla)+1);
	strcpy(nombreTablaMayuscula,paquete_select->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

		if(existeTabla(nombreTablaMayuscula)){
			t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
			int particionKey;
			particionKey =	particionDeKey(paquete_select->valor_key,metadataDeTabla->particiones);
			t_list* RegistrosEncontrados=list_create();
			void* RegistroParticion= buscarEnParticion(nombreTablaMayuscula,particionKey,paquete_select->valor_key);
			void* RegistroMemTable = buscarMemTable(nombreTablaMayuscula,paquete_select->valor_key); // 8 bytes ml
			void* RegistroTemporal= buscarEnTemporales(nombreTablaMayuscula,paquete_select->valor_key);
			if(RegistroMemTable!=NULL){
			list_add(RegistrosEncontrados,RegistroMemTable);
			}
			if(RegistroParticion!=NULL){
			list_add(RegistrosEncontrados,RegistroParticion);

			}
			if(RegistroTemporal!=NULL){
			list_add(RegistrosEncontrados,RegistroTemporal);

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

t_metadata* APIdescribe (t_paquete_describe* paquete_describe){
	char nombreTablaMayuscula [strlen(paquete_describe->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_describe->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

	if(existeTabla(nombreTablaMayuscula)){
		t_metadata* metadataDeTabla = obtenerMetadataTabla(nombreTablaMayuscula);
		return metadataDeTabla;
		free(metadataDeTabla);
	} else {
		error_show("NO EXISTE %s\n",nombreTablaMayuscula);
		return NULL;
	}

}

void APIdescribeTodasLasTablas(){
	t_list* listaDeTablas = listarTablasExistentes();
	if(listaDeTablas != NULL){
		list_iterate(listaDeTablas,imprimirMetadataDeTabla);
	}
	list_destroy(listaDeTablas);
	//DESPUES VAMOS A TENER QUE CAMBIAR A TIPO T_LIST LA FUNCION ASI RETORNAMOS LA LISTA
	//Y VEMOS COMO LA ENVIAMOS.
}

void listarTablas(){
	DIR *d;
	  struct dirent *dir;
	  d = opendir("/home/utnso/LISSANDRA_FS/Tables");
	  if (d)
	  {
		while ((dir = readdir(d)) != NULL) {
			if (!string_contains(dir->d_name,".")) printf("%s ", dir->d_name);

		}
		closedir(d);
	  }
}

void imprimirMetadata(t_metadata* metadataDeTablaPedida){
	printf("%s%s ",BLUE,pasarAConsistenciaChar(metadataDeTablaPedida->consistencia));
	printf(" %d ",metadataDeTablaPedida->particiones);
	printf(" %d\n",metadataDeTablaPedida->tiempo_de_compactacion);
	printf("%s",NORMAL_COLOR);
}



t_list* listarTablasExistentes() {
  DIR *d;
  struct dirent *dir;
  d = opendir("/home/utnso/LISSANDRA_FS/Tables");
  t_list *listaDeTablas = list_create();
  if (d)
  {
    while ((dir = readdir(d)) != NULL) {
    	if (!string_contains(dir->d_name,".")){
    	list_add(listaDeTablas,dir->d_name);
    	}

    }
    return listaDeTablas;
    closedir(d);
  } else return listaDeTablas;

  	list_destroy(listaDeTablas);
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
	registroNuevo->value=malloc(strlen(value)+1);
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


char* DirectorioDeTemporalNuevo(char* nombretabla){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);

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

		t_config* directorioTemporal= config_create(directorioAux);
		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorioTemporal,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		remove(directorioAux);
		i++;
		strcpy(directorioAux,directorioTablas);
		sprintf(buffer,"%d",i);
		strcat(directorioAux,buffer);
		strcat(directorioAux,".tmp");
	}

	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".tmp");
	free(directorioAux);
	return directorioTablas;

}


char* DirectorioDeTemporal(char* nombretabla){

	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);

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
	return directorioTablas;


}

char* DirectorioDeTemporal(char* nombretabla,int nroTemporal){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);

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
	return directorioTablas;


}




char* DirectorioDeBloque(int numeroBloque){ //0 ml
	t_config* config = config_create("Lissandra.config");
	char* buffer=malloc(sizeof(int));
	sprintf(buffer,"%d",numeroBloque);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioBloque = malloc(strlen(Montaje) + strlen(".bin") +strlen("Bloques/") +sizeof(buffer)+1);
	strcpy(directorioBloque,Montaje);
	strcat(directorioBloque,"Bloques/");
	strcat(directorioBloque,buffer);
	strcat(directorioBloque,".bin");
	config_destroy(config);
	free(buffer);
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
		{ //BORRAR DE LA MEMTABLE.
		dictionary_remove_and_destroy(memTable,nombreTabla,liberarNodo);
		printf("Se eliminaron los registros de %s de la memTable.\n",nombreTabla);
		} else {printf("No se hallaron registros en la memTable de %s\n",nombreTabla);}

	}
}

void RemoverParticionesDeTablaYSusBloques(char* nombreTabla){
	t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTabla);
	int particiones = metadataDeTabla->particiones;
	free(metadataDeTabla);

	for(int i=1;i<=particiones;i++){

		char *directorioDeTablaABorrar= DirectorioDeParticion(nombreTabla,i);
		t_config* directorio = config_create(directorioDeTablaABorrar);
		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorio,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		//ESTA FUNCION REMUEVE CADA BLOQUE ASIGNADO A ESA PARTICION.

		remove(directorioDeTablaABorrar); //ESTE REMOVE REMUEVE LA PARTICION.

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

					free(registrosCompletos);
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

		for(int cerrar=0;registrosSeparados[cerrar];cerrar++)
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


		for(int cerrar=0;registrosSeparados[cerrar];cerrar++)
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
free(directorioTemporal);
temporales++;
directorioTemporal= DirectorioDeTemporal(nombreTabla,temporales);
}

free(directorioTemporal);
if(temporales==1) {return NULL;}

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
		}
		bool _filtradoKey(void* elemento){
			return filtrarPorKeyRegistro(elemento,key);
		}
		if(!listaVacia(listaDeRegistros)){
		t_list* listaRegistrosKey = list_filter(listaDeRegistros,_filtradoKey);
		if(!listaVacia(listaRegistrosKey)){
		list_sort(listaRegistrosKey,mayorTimeStampRegistro);

		t_registro* AuxregistroEncontrado = list_remove(listaRegistrosKey, 0); // 0 es el primero
		t_registro* registroEncontrado = crearRegistro(AuxregistroEncontrado->value,AuxregistroEncontrado->key,AuxregistroEncontrado->timestamp);
		list_clean_and_destroy_elements(listaRegistrosKey,liberarBloque);
		list_clean_and_destroy_elements(listaDeRegistros,liberarBloque);
		free(todosLosRegistrosTemporales);
		free(arrayRegistros);
		//free(AuxregistroEncontrado);
		return registroEncontrado;
		}
		list_destroy(listaRegistrosKey);
		}
		list_destroy(listaDeRegistros);
		free(todosLosRegistrosTemporales);
		free(arrayRegistros);
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
