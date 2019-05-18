

#include "APIS.h"


void APIcreate(t_paquete_create* paquete_create){

	char nombreTablaMayuscula [strlen(paquete_create->nombre_tabla)+1];
	string_to_upper(nombreTablaMayuscula);
	strcpy(nombreTablaMayuscula,paquete_create->nombre_tabla);

	if(!existeTabla(nombreTablaMayuscula)){
		crearTabla(nombreTablaMayuscula);
		crearMetadataConfig(nombreTablaMayuscula,paquete_create->metadata.consistencia,paquete_create->metadata.particiones,paquete_create->metadata.tiempo_de_compactacion);
		crearParticiones(nombreTablaMayuscula,paquete_create->metadata.particiones);

	} else if (EEXIST==errno) error_show("EXISTE LA TABLA");

}


void APIinsert(t_paquete_insert* paquete_insert){
	/*
		 *1. Verificar que la tabla exista en el file system. En caso que no exista, informa el error y continúa su ejecución.
		 *2. Obtener la metadata asociada a dicha tabla.
		 *3. Verificar si existe en memoria una lista de datos a dumpear. De no existir, alocar dicha memoria.
		 *4. El parámetro Timestamp es opcional. En caso que un request no lo provea (por ejemplo insertando un valor desde la consola), se usará el valor actual del Epoch UNIX.
		 *5. Insertar en la memoria temporal del punto anterior una nueva entrada que contenga los datos enviados en la request.

		 * En los request solo se utilizarán las comillas (“”) para enmascarar el Value que se envíe. No se proveerán request con comillas en otros puntos.
	 */
	char nombreTablaMayuscula[strlen(paquete_insert->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_insert->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);
	if(existeTabla(nombreTablaMayuscula)){
	agregarTabla(paquete_insert);
	imprimirListaTablas();

	//free(nuevaTabla);
	//free(nuevoRegistro);

	} else error_show("NO EXISTE LA TABLA");


}

char* APIselect(t_paquete_select* paquete_select){
	/*
	 *1. Verificar que la tabla exista en el file system.
	 *2. Obtener la metadata asociada a dicha tabla.
	 *3. Calcular cual es la partición que contiene dicho KEY.
	 *4. Escanear la partición objetivo, todos los archivos temporales y la memoria temporal de dicha tabla (si existe) buscando la key deseada.
	 *5. Encontradas las entradas para dicha Key, se retorna el valor con el Timestamp más grande.
	 *
	 */
	char nombreTablaMayuscula[strlen(paquete_select->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_select->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

		if(existeTabla(nombreTablaMayuscula)){
			t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
			int particionKey;
			particionKey =	particionDeKey(paquete_select->valor_key,metadataDeTabla->particiones);
			t_list* RegistrosEncontrados=list_create();
			list_add(RegistrosEncontrados,buscarMemTable(nombreTablaMayuscula,paquete_select->valor_key));
			//list_add(RegistrosEncontrados,buscarParticiones(nombreTablaMayuscula,paquete_select->valor_key));
			//list_add(RegistrosEncontrados,buscarTemporal(nombreTablaMayuscula,paquete_select->valor_key));
			char* valueEncontrado = elegirMayorTimeStamp(RegistrosEncontrados);
			printf("El Value encontrado fue %s\n",valueEncontrado);
			free(metadataDeTabla);
			list_clean(RegistrosEncontrados);
			return valueEncontrado;

		}else return NULL;

}

char* elegirMayorTimeStamp(t_list* RegistrosEncontrados){
	list_sort(RegistrosEncontrados,mayorTimeStamp);
	t_registro* registroConMayorTimeStamp = (t_registro*)list_remove(RegistrosEncontrados,0);
	return registroConMayorTimeStamp->value;
}

t_registro* crearRegistro (char*value,uint16_t key,long long timestamp){
	t_registro* registroNuevo=malloc(sizeof(t_registro));
	registroNuevo->value=malloc(strlen(value));
	strcpy(registroNuevo->value,value);
	registroNuevo->key=key;
	registroNuevo->timestamp=timestamp;
	return registroNuevo;
}

int particionDeKey(int key,int particiones){
	return particiones%key;

}

t_metadata* obtenerMetadataTabla (char* nombreTabla ){

	char* ubicacionMetaData = DirectorioDeMetadataTabla(nombreTabla);
	t_config* config = config_create(ubicacionMetaData);
	char* consistencia = config_get_string_value(config,"CONSISTENCY");
	int particiones = atoi(config_get_string_value(config,"PARTITIONS"));
	int compactacion = atoi(config_get_string_value(config,"COMPACTION_TIME"));
	t_metadata* metadataObtenida=crearMetadata(pasarAConsistenciaINT(consistencia),particiones,compactacion);
	free(ubicacionMetaData);
	return metadataObtenida;
	config_destroy(config);
}


int existeTabla(char* TablaBuscada){

	char* directorioTablas = DirectorioDeTabla(TablaBuscada);
	//directorioTablas termina siendo el directorio en especifico buscado.
	//por ejemplo "/home/utnso/lfs/LissandraFileSystem/Tablas/TablasA"

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
		/* Fallo por otra razon */
		printf("Fallo desconocido :(\n");
		free(directorioTablas);
	    return 0;
	}
}

t_metadata* crearMetadata(consistency consistencia, int particiones,int tiempo_compactacion){
	t_metadata* metadata = malloc(sizeof(metadata));

	metadata->consistencia=consistencia;
	metadata->particiones=particiones;
	metadata->tiempo_de_compactacion=tiempo_compactacion;

	return(metadata);

}

//PASAR CONSISTENCIAS
int pasarAConsistenciaINT(char* consistencia){

	if (strcmp(consistencia, "SC")==0) {
		return 0;
	}
	else if (strcmp(consistencia, "SHC")==0) {
		return 1;
	}
	else if (strcmp(consistencia, "EC")==0) {
		return 2;

	}

}

char* pasarAConsistenciaChar(int consistencia){

	if (consistencia==0) {
		return "SC";
	}
	else if (consistencia==1) {
		return "SHC";
	}
	else if (consistencia==2) {
		return "EC";
	}

}

void logLaTablaYaExiste(){
	t_log* logger = iniciar_logger();
	log_info(logger, "ERROR LA TABLA YA EXISTE");
    log_destroy(logger);
}

void crearTabla(char* nombreTabla){
	char* DirectorioTablas = DirectorioDeTabla(nombreTabla);
	mkdir(DirectorioTablas,0777);
	free(DirectorioTablas);
}


char* DirectorioDeTabla(char* tabla){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(tabla) +strlen("Tables/") +1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,tabla);
	return directorioTablas;


}
char* DirectorioDeMetadataTabla(char* tabla){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(tabla) +strlen("Tables/") + strlen("/Metadata.config") +1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,tabla);
	strcat(directorioTablas,"/Metadata.config");
	return directorioTablas;

}
char* DirectorioDeMetadata(){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioMetadata = malloc(strlen(Montaje)  +strlen("Metadata/Metadata.bin")  +1);
	strcpy(directorioMetadata,Montaje);
	strcat(directorioMetadata,"Metadata/Metadata.bin");
	return directorioMetadata;

}
char* DirectorioDeParticion(char* nombretabla,int numeroParticion){
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
	return directorioTablas;


}

void crearMetadataConfig(char*nombreTablaMayuscula, consistency consistencia, int particiones,int tiempo_compactacion){
	char *directorioTablaNueva = DirectorioDeMetadataTabla(nombreTablaMayuscula);
	FILE *fp=NULL;
	fp=fopen(directorioTablaNueva,"a");
	fprintf(fp,"CONSISTENCY=%s\nPARTITIONS=%d\nCOMPACTION_TIME=%d",pasarAConsistenciaChar(consistencia),particiones,tiempo_compactacion);
	fclose(fp);
	free(directorioTablaNueva);
}

char* DirectorioBitMap(){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioMetadata = malloc(strlen(Montaje)  +strlen("Metadata/bitmap.bin")  +1);
	strcpy(directorioMetadata,Montaje);
	strcat(directorioMetadata,"Metadata/bitmap.bin");
	return directorioMetadata;
}


void crearBitMap(){
	t_config* config = config_create(DirectorioDeMetadata());
	char* blockNum = config_get_string_value(config,"BLOCKS");
	t_bitarray *bitmap;
	bitmap=bitarray_create_with_mode("\n",512,0); // aca va la cantidad de 0 necesarios dividido 8
	FILE *fp=NULL;
	fp=fopen( DirectorioBitMap(),"a");
	int i = 0;
	while(i<4096){
		//bitarray_set_bit(bitmap, i);
		int valor = bitarray_test_bit(bitmap, i);
		fprintf(fp,"%d",valor);
	//bitarray_clean_bit(bitmap, 1);
	//printf("%d\n",bitPrueba);
	i++;}
	fclose(fp);
	bitarray_destroy(bitmap);
}

void limpiarBitMap(){ // cuando creas el bitmap puede venir con cualquier valor
	FILE *fp=NULL;
	fp=fopen( DirectorioBitMap(),"a+");
	char i;
	while((i = fgetc(fp)) !=EOF){
		if (i == 1){ i=0; fprintf(fp,"%d",i);}
		printf("%c",i);

	}
}


void crearParticiones(char*nombreTabla ,int particiones){
	FILE *fp=NULL;
	FILE *particion=NULL;
	char bitmap[4096];

	fp=fopen(DirectorioBitMap(),"r+");
	fseek(fp,0,SEEK_SET);
	fread(bitmap,sizeof(char),1,fp);
	int i=0;
	while(particiones){
		if(bitmap[i]=='0'){
		particiones=particiones-1;
		bitmap[i]='1';
		particion=NULL;
		particion=fopen(DirectorioDeParticion(nombreTabla,i),"w");
		fprintf(particion,"SIZE=0\nBLOCKS=[%d]",i);

		}
		i++;

	}
	fclose(particion);
	i=0;
	while(i<4096){
	fprintf(fp,"%c",bitmap[i]);
	i ++;
	}
	fclose(fp);
}


	//free(nombreTablaMayus);




