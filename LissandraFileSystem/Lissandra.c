#include "Lissandra.h"

t_dictionary* memTable=NULL;
t_bitarray* bitmap;
int main(void)
{
	//crearArchivoBitmap();
	crearBitMap();
	pthread_t consola;//, atender[20];
	logger = iniciar_logger();
	pthread_t recibidorDePaquetes;
	//crearArchivoBitmap();

//	int cantidadDeRequest =0;
//	argumentosEnvioPaquete* argumentos[20];
	pthread_create(&consola,NULL,prenderConsola,NULL);

//	int server_fd;
//	int cliente_fd;
//	server_fd = iniciar_servidor();

	//int *arg = malloc(sizeof(*arg));
//	while(1)
//	{
//
//		if(cliente_fd = esperar_cliente(server_fd))
//		{
//
//			*arg = cliente_fd;
//
//			if(pthread_create(&recibidorDePaquetes,NULL,recibir_paquetes,arg))
//			   printf("Error Creando hilo.\n");
//
//			else
//			   printf("Se disparo el hilo.\n");
//
//			pthread_join(recibir_paquetes,NULL);
//
//		}

//	argumentos[cantidadDeRequest]= crearArgumentosDeConexion(cliente_fd, server_fd);
//	pthread_create(&atender[cantidadDeRequest],NULL,(void*)recibir_paquetes,(void*)argumentos);
	//recibir_paquetes(argumentos);

//	}
//	close(cliente_fd);
//
////	pthread_join(atender[cantidadDeRequest], (void**)NULL);
////	}
//
	log_info(logger, "FIN CONEXION");
	pthread_join(consola, (void**)NULL);
	return EXIT_SUCCESS;
}


//
//argumentosEnvioPaquete* crearArgumentosDeConexion(int clientefd, int serverfd){
//	struct argumentosEnvioPaquete *infoConexion =malloc(sizeof(argumentosEnvioPaquete));
//	infoConexion->clientefd =clientefd;
//	infoConexion->serverfd =serverfd;
//	return infoConexion;
//}
