/*
 * bloque.c
 *
 *  Created on: 21 may. 2019
 *      Author: utnso
 */


#include"bloque.h"

void crearBloque(int nroBloque){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBloque=DirectorioDeBloque(nroBloque);
	t_config* config = config_create(directorioMetadata);
	int tamanioBloque = atoi(config_get_string_value(config,"BLOCK_SIZE"));
	FILE *fp;

	//printf("\n");
	fp=fopen(directorioBloque,"w");
	ftruncate(fp,tamanioBloque);
	fclose(fp);
	free(directorioBloque);
	free(directorioMetadata);
	config_destroy(config);
}



void escribirEnBloque(int nroBloque, char* registro, bool ContienefinalRegistro){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBloque=DirectorioDeBloque(nroBloque);
	t_config* config = config_create(directorioMetadata);
	int tamanioBloque = atoi(config_get_string_value(config,"BLOCK_SIZE"));
	int fd = open(directorioBloque, O_RDWR|O_CREAT|O_APPEND, (mode_t)0600);
	ftruncate(fd,tamanioBloque);
	char* archivoBloque;// =malloc(tamañoBloque);

	archivoBloque=(char*) mmap(0,tamanioBloque, PROT_READ|PROT_WRITE, MAP_SHARED ,fd,0);
		if(archivoBloque==MAP_FAILED){
			printf("ERROR AL MAPEAR A MEMORIA");
			close(fd);
	}
	strcat(archivoBloque,registro);
	if(ContienefinalRegistro)
	strcat(archivoBloque,"\n");
	//if(string_ends_with(archivoBloque,"\n")) printf("ultimo caracter salto de linea");
	char* RegistrosBloque=malloc(strlen(archivoBloque)+1);

	strcpy(RegistrosBloque,archivoBloque);
	printf("%d",string_length(RegistrosBloque));
	msync(archivoBloque,tamanioBloque,MS_SYNC);
	munmap(archivoBloque,tamanioBloque);
	close(fd);
	free(directorioMetadata);
	free(directorioBloque);
	config_destroy(config);
	printf("Prueba: %s",RegistrosBloque);
}

char* serializarRegistro(char* value,uint16_t key,long long timestamp){

	char aux[256];
	sprintf(aux,"%lli",timestamp);
	//char* timeChar= string_itoa(timestamp);
	char* keyChar=string_itoa(key);
	char* registro = malloc(strlen(value)+strlen(aux)+strlen(keyChar)+2);

	strcpy(registro,aux);
	strcat(registro,";");
	strcat(registro,keyChar);
	strcat(registro,";");
	strcat(registro,value);
	//printf("%s",registro);
	return registro;


}
