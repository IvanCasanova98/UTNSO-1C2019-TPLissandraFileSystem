#include "memorias.h"

t_dictionary* tabla_metadata;

void startup(){
	tabla_metadata = dictionary_create();

}

void agregar_datos(char* nombre_tabla, char* consistencia){
	dictionary_put(tabla_metadata,nombre_tabla,consistencia);
}

bool existe_tabla(char* nombre_tabla){
	return dictionary_has_key(tabla_metadata, nombre_tabla);
}

void deserealizar_metadata(int socket){
	int cant_elementos;
	recv(socket,&cant_elementos,sizeof(int),MSG_WAITALL);

	int i=0;
	while(i<cant_elementos){
		t_metadata* nodo_metadata = deserealizar_nodo(socket);
		agregar_datos(nodo_metadata->nombre_tabla, nodo_metadata->consistencia);

		i++;
	}

//	dictionary_iterator(tabla_metadata,imprimir_diccionario);

}

void imprimir_diccionario(char* nombre_tabla,char*consistencia){
	printf("\nNombre: %s   Consistencia: %s\n", nombre_tabla,consistencia);
}


