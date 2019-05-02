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

typedef struct t_paquete_insert
{
	char nombre_tabla[7];
	int valor_key;
	char value[20];
	int timestamp;
}__attribute__((packed)) t_paquete_insert;

t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
t_paquete_select* deserializar_paquete_select(int socket_cliente);
t_paquete_insert* deserializar_paquete_insert(int socket_cliente);

//---------------------CREAR PAQUETE

t_paquete_select* crear_paquete_select(char *nombretabla,int valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp);


//---------------------LOGGEAR PAQUETE

void loggear_paquete_select(t_paquete_select* paquete);
void loggear_paquete_insert(t_paquete_insert* paquete);





#endif /* PAQUETE_H_ */
