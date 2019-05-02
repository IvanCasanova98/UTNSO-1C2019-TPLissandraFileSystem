#ifndef PAQUETE_H_
#define PAQUETE_H_
#define TAMANIO_NOMBRE_TABLA 7


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>


typedef struct t_paquete_select
{
	char nombre_tabla[7];
	int valor_key;
}__attribute__((packed)) t_paquete_select;


t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
t_paquete_select* deserializar_paquete_select(int socket_cliente);








#endif /* PAQUETE_H_ */
