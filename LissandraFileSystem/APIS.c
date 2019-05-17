

#include "APIS.h"


void APIcreate(t_paquete_create* paquete_create){

	char nombreTablaMayuscula [strlen(paquete_create->nombre_tabla)+1];
	string_to_upper(nombreTablaMayuscula);
	strcpy(nombreTablaMayuscula,paquete_create->nombre_tabla);
//	int i =0;
//	while(nombreTablaMayuscula[i]){nombreTablaMayuscula[i]= (unsigned char)toupper(nombreTablaMayuscula[i]); i++;}

	if(!existeTabla(nombreTablaMayuscula)){
		crearTabla(nombreTablaMayuscula);
		crearMetadataConfig(nombreTablaMayuscula,paquete_create->metadata.consistencia,paquete_create->metadata.particiones,paquete_create->metadata.tiempo_de_compactacion);
		crearParticiones(nombreTablaMayuscula,paquete_create->metadata.particiones);

}else if(EEXIST==errno){
	error_show("EXISTE LA TABLA");}

}


void APIinsert(t_paquete_insert* paquete_insert){

	char nombreTablaMayuscula[strlen(paquete_insert->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_insert->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);
	//char* nombreTablaMayuscula = realloc(paquete_insert->nombre_tabla, strlen(paquete_insert->nombre_tabla)+1);

	if(existeTabla(nombreTablaMayuscula)){

	nodoTablaMemTable* nuevaTabla =	crearNodoTabla(nombreTablaMayuscula);
	nodoRegistroMemTable* nuevoRegistro = crearNodoRegistro(paquete_insert->value,paquete_insert->valor_key,paquete_insert->timestamp);
	agregarTabla(nuevaTabla,nuevoRegistro);
	//agregarRegistro(nuevaTabla,nuevoRegistro);
	imprimirRegistrosTabla(nuevaTabla);
	//imprimirListaTablas();


	} else error_show("NO EXISTE LA TABLA");


}





//herramientasDeApis las usamos para facilitar la declaratividad de las funciones apis
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

metadata* crearMetadata(consistency consistencia, int particiones,int tiempo_compactacion){
	metadata* metadata = malloc(sizeof(metadata));

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
	t_bitarray *bitmap=malloc(512);
	bitmap=bitarray_create_with_mode("00000000",512,0);
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




