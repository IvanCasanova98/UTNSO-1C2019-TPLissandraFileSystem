#include "paquete.h"

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		puts(" *** No se recibio correctamente el COD OP ***");
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		//EN la variable tamanio, mete el primer int del buffer.
		//SE desplaza hacia el fin de ese primer int.
		//reserva un *char de ese tamanio que indica el primer int.
		//copia en valor todo el mensaje que sigue luego del header.
		//El desplazamiento pasa a ser del tamanio indicado en el header.

		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	return NULL;
}

//-------------------func prueba

//char* nombre_tabla(int socket_cliente){
//
//	char* nombre_tabla[20];
//	recv(socket_cliente, &nombre_tabla, sizeof(int), MSG_WAITALL);
//
//	return *nombre_tabla;
//
//}


void deserializar_paquete_select(int socket_cliente, t_paquete_select *paquete ){


	void *buffer = malloc(sizeof(t_paquete_select));
	int desplazamiento = 0;
	/*
	 * Queria recibir el paquete entero.
	 * EL peso del paquete entero seria 11 (char[7] + 4 del int).
	 * Esto se podria hacer simplemente con un:
	 * recv(socket_cliente, buffer, sizeof(t_paquete_select),MSG_WAITALL);
	 * pero no recibe todo. en los printfs de abajo se ve como solo por alguna razon
	 * solo recibe 4.
	 */
	recv(socket_cliente, buffer, sizeof(t_paquete_select),MSG_WAITALL);

		printf("El tamanio de un t_paquete_select es: %d\n",sizeof(t_paquete_select));
		printf("Se recibio en el buffer: %d\n", sizeof(buffer));

		memcpy(&(paquete->nombre_tabla),buffer + desplazamiento,sizeof(char[TAMANIO_NOMBRE_TABLA]));
		desplazamiento+= sizeof(char[TAMANIO_NOMBRE_TABLA]);

		memcpy(&(paquete->valor_key),buffer + desplazamiento, sizeof(paquete->valor_key));
		desplazamiento+= sizeof(paquete->valor_key);



	free(buffer);

}


void nombre_tabla(int socket_cliente){

	char nombre_tabla[20];
	recv(socket_cliente, &nombre_tabla, sizeof(char[20]), MSG_WAITALL);

	printf("EL nombre de la tabla recibida es: %s\n", nombre_tabla);

}

int valor_key(int socket_cliente){

	int valor_key;
	recv(socket_cliente, &valor_key, sizeof(int), MSG_WAITALL);

	printf("EL KEY VALUE recibido es: %d\n", valor_key);
	return valor_key;

}
