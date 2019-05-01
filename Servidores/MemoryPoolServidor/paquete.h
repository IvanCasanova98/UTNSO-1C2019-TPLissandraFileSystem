#ifndef PAQUETE_H_
#define PAQUETE_H_
#define TAMANIO_NOMBRE_TABLA 7


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>


t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

//BEHRI

/*
 * ME tira que no reconoce op_code
 * voy a redefinirlo llamandolo int en el struct: t_paquete_select
 */


typedef struct
{
//	op_code codigo_operacion;
//	int codigo_operacion;
	char nombre_tabla[TAMANIO_NOMBRE_TABLA];
	int valor_key;
//	t_buffer* buffer;
}__attribute__((packed)) t_paquete_select;

void deserializar_paquete_select(int,t_paquete_select*);

//BEHRI

//---func prueba
void nombre_tabla(int socket_cliente);
int valor_key(int socket_cliente);

#endif /* PAQUETE_H_ */
