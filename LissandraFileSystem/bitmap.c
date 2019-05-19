/*
 * bitmap.c
 *
 *  Created on: 18 may. 2019
 *      Author: utnso
 */
#include "bitmap.h"
t_bitarray *bitmap;


void crearBitMap(){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	int bytes = BIT_CHAR(blockNum);
	char* bitsVacios = string_repeat(0,bytes);
	t_bitarray* bitmap= bitarray_create_with_mode(bitsVacios,bytes,0);

	int fd = open(directorioBitmap, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);

	ftruncate(fd,blockNum); //truncate le da un tamaño al .bin en este caso es el numero de bloques ya que sizeof(char)= 1 -> blockNum*sizeof(char)=blockNum, si usaramos int seria  blockNum*sizeof(int)
	/*/ basicamente mmap pasa todo lo que esta dentro de un txt y lo convierte en una variable. Ponele 1.bin tiene un "HOla" y lo pasa a un (void*) en este caso use char pq ya sabia que los bitmap se guardan con char
	 * pero mas adelante (value;key;timestamp) hay que inicializarlo como (void*) y despues ir casteando campo por campo (char*;int;long long)*/
	char* archivoBitmap = (char*)mmap(0,blockNum, PROT_READ|PROT_WRITE, MAP_SHARED ,fd,0);
	char* valores =malloc(blockNum);
	strcpy(valores,"");
	int valorAux;
	for (int i = 0; i<blockNum;i++){
	valorAux =bitarray_test_bit(bitmap,i);
	strcat(valores , string_itoa(valorAux));
	}
	printf("%s",valores);
	strcpy(archivoBitmap,valores);
	msync(archivoBitmap,blockNum,MS_SYNC);
	munmap(archivoBitmap,blockNum);
	close(fd);
	free(directorioMetadata);
	free(directorioBitmap);
	free(valores);
	config_destroy(config);
}


void ObtenerBitmap(){
	char* directorioMetadata = DirectorioDeMetadata();
	char* directorioBitmap = DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));

	int bytes = BIT_CHAR(blockNum);

	int fd= open(directorioBitmap,O_RDONLY);

	ftruncate(fd,blockNum);

	void* pmap = mmap(0,blockNum,PROT_READ,MAP_PRIVATE,fd,0);
	//prinf("%s",pmap);

	char* bitmapChar =pmap;
	;
	//string_trim_right(pmap);
	// string_trim_right
	//strcpy(bitmapChar,pmap);

	bitmap= bitarray_create_with_mode(eliminarBarra0(bitmapChar),bytes,0);


	int i = 0;
		while(i<blockNum){
			//bitarray_set_bit(bitmap, i);
			int valor = bitarray_test_bit(bitmap, i);
			printf("%d",valor);
			//bitarray_clean_bit(bitmap, 1);

		i++;}
	//free(bitmapChar);
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
