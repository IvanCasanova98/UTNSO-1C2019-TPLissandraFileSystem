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
	while(blockNum%8!=0){
		blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	int bytes = BIT_CHAR(blockNum);
	char* bitsVacios = string_repeat(0,bytes);

	FILE *fp;
	t_bitarray* arrayCreador = bitarray_create_with_mode(bitsVacios,bytes,0);
	for (int i = 0; i<blockNum;i++){
		printf("%d",bitarray_test_bit(arrayCreador,i));
	}
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
	while(blockNum%8!=0){
			blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	int bytes = BIT_CHAR(blockNum);
	char* archivoBitmap;
	int fd = open(directorioBitmap, O_RDWR , (mode_t)0600);
	struct stat mystat;
	if(fstat(fd,&mystat)<0){
		printf("Error al establecer fstat\n");
		close (fd);
	}

	archivoBitmap=(char*) mmap(0,mystat.st_size, PROT_READ|PROT_WRITE, MAP_SHARED ,fd,0);
	if(archivoBitmap==MAP_FAILED){
		printf("ERROR AL MAPEAR A MEMORIA");
		close(fd);

	}


	//bitmap=malloc(mystat.st_size);
	bitmap=bitarray_create_with_mode(archivoBitmap , mystat.st_size ,0);

//
//	for (int i = 0; i<blockNum;i++){
//		printf("%d",bitarray_test_bit(bitmap,i));
//
//}



	msync(archivoBitmap,bytes,MS_SYNC);
	//munmap(archivoBitmap,bytes);
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
	while(blockNum%8!=0){
			blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	int bytes = BIT_CHAR(blockNum);
	FILE *fp;
	fp=fopen(directorioBitmap,"w");
	fwrite(bitmap->bitarray,1,bytes,fp);


	fclose(fp);
	free(directorioMetadata);
	free(directorioBitmap);
	config_destroy(config);


}

//
//int existeBitmap(){
//
//	char* directorioBitMap = DirectorioBitMap();
//	FILE*file;
//	if(file=fopen(directorioBitMap,"r")){
//		fclose(file);
//		free(directorioBitMap);
//		return 1;
//	}
//free(directorioBitMap);
//return 0;
//}


void pruebasSet(){
	//bitarray_set_bit(bitmap, 5);
	int i=0;
	while(i<512) {printf("%d",i);i++;};
	//ActualizarBitmap();
	//printf("%d",bitarray_test_bit(bitmap,5));


}



