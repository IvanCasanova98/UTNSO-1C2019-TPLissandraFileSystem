/*
 * bitmap.c
 *
 *  Created on: 18 may. 2019
 *      Author: utnso
 */
#include "bitmap.h"
t_bitarray *bitmap ;

void crearArchivoBitmap(){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	int bytes = BIT_CHAR(blockNum);
	char* bitsVacios = string_repeat(0,bytes);

	FILE *fp;
	t_bitarray* arrayCreador = bitarray_create_with_mode(bitsVacios,bytes,0);
//	for (int i = 0; i<blockNum;i++){
//		printf("%d",bitarray_test_bit(arrayCreador,i));
//	}
	printf("\n");
	fp=fopen(directorioBitmap,"w");
	fwrite(arrayCreador->bitarray,1,bytes,fp);
	fclose(fp);
	free(directorioMetadata);
	free(directorioBitmap);
	config_destroy(config);

}


void crearBitMap(){


	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	int bytes = BIT_CHAR(blockNum);

	int fd = open(directorioBitmap, O_RDWR , (mode_t)0600);

	ftruncate(fd,bytes);

	char* archivoBitmap =(char*) mmap(0,bytes, PROT_READ|PROT_WRITE, MAP_SHARED ,fd,0);

	//printf(archivoBitmap-)


	bitmap=bitarray_create_with_mode(archivoBitmap , bytes,0);

	//memmove(archivoBitmap,bitmap,bytes);
	//memmove(archivoBitmap,bitarray_create_with_mode(bitsVacios,bytes,0),bytes);



	//memcpy(bitmap,archivoBitmap,bytes);

//	for (int i = 0; i<blockNum;i++){
//		printf("%d",bitarray_test_bit(bitmap,i));
//
//	}



	msync(archivoBitmap,bytes,MS_SYNC);
	munmap(archivoBitmap,bytes);
	close(fd);
	free(directorioMetadata);
	free(directorioBitmap);

	config_destroy(config);
}


void ActualizarBitmap(){


	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	int bytes = BIT_CHAR(blockNum);
	char* bitsVacios = string_repeat(0,bytes);
	FILE *fp;
	fp=fopen(directorioBitmap,"w");
	fwrite(bitmap->bitarray,1,bytes,fp);


	fclose(fp);
	free(directorioMetadata);
	free(directorioBitmap);
	config_destroy(config);


}

char* eliminarBarra0(char* nuevoChar){
	int i=0;
	int longitud=strlen(nuevoChar);
	nuevoChar[longitud] = "";
	return nuevoChar;
}

int existeBitmap(){

	char* directorioBitMap = DirectorioBitMap();
	FILE*file;
	if(file=fopen(directorioBitMap,"r")){
		fclose(file);
		free(directorioBitMap);
		return 1;
	}
free(directorioBitMap);
return 0;
}
