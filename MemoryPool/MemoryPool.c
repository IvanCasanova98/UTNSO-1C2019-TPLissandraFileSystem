//incluir dos logs

#include "MemoryPool.h"

int main(void)

{
	pthread_t hilo_servidor;
	pthread_t hilo_consola;

	pthread_create(&hilo_servidor, NULL, servidor, NULL); //Falta verificacion en la creacion de hilos
	pthread_create(&hilo_consola, NULL, ingresar_paquete, NULL);

	pthread_join(hilo_servidor,NULL);
	pthread_join(hilo_consola,NULL);

	return 1;
}
