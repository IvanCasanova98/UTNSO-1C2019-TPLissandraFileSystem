#include "Lissandra.h"


t_dictionary* TablasCompactacion=NULL;
t_dictionary* TablasSem=NULL;
t_dictionary* memTable=NULL;
t_bitarray* bitmap;
t_list* ListaSem=NULL;
bool estaDump =0;
int main(void)
{

	t_config* config = leer_config();
	//crearMetadataDePuntoDeMontaje();
	//crearArchivoBitmap();
	crearBitMap();
	pthread_t consola,dumpeo,dump,compactar;

	logger = iniciar_logger();
	int tiempoEntreDump = atoi(config_get_string_value(config, "TIEMPO_DUMP"));
	//gettimeofday(&tiempoHastaDump,NULL);


	//levantarHilosTablasExistentesCompactacion();
	pthread_create(&dump,NULL,chekearDumpeo,NULL);
	pthread_create(&consola,NULL,prenderConsola,NULL);


//	// ----------------------------------------------------------------------------
//	int socket_servidor;
//
//		char* ip = config_get_string_value(config, "IP");
//		char* puerto = config_get_string_value(config, "PUERTOESCUCHA");
//
//	    struct addrinfo hints, *servinfo, *p;
//
//	    memset(&hints, 0, sizeof(hints));
//	    hints.ai_family = AF_UNSPEC;
//	    hints.ai_socktype = SOCK_STREAM;
//	    hints.ai_flags = AI_PASSIVE;
//
//	    getaddrinfo(ip, puerto, &hints, &servinfo);
//
//	    for (p=servinfo; p != NULL; p = p->ai_next)
//	    {
//	        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
//	            continue;
//
//	        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
//	            close(socket_servidor);
//	            continue;
//	        }
//	        break;
//	    }
//
//		listen(socket_servidor, 5);
//
//	    freeaddrinfo(servinfo);
//
//		log_info(logger, "ESPERANDO MEMORIA");
//
//
//		struct sockaddr_in dir_cliente;
//			int tam_direccion = sizeof(struct sockaddr_in);
//
//			int socket_cliente;
//
//			log_info(logger, "INICIO CONEXION");
//		// ----------------------------------------------------------------------------
//
//
//	    while ((socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion)))
//
//		{
//			printf("NUEVA CONEXION ACEPTADA.\n");
//
//			pthread_t recibidorPaquetes;
//			int *arg = malloc(sizeof(*arg));
//			*arg = socket_cliente;
//
//			//chekearDumpeo()
//			pthread_create(&recibidorPaquetes,NULL,recibir_paquetes,arg);
//
//		}

	config_destroy(config);
	log_info(logger, "FIN CONEXION");

	pthread_join(dump, (void**)NULL);
	pthread_join(consola, (void**)NULL);
	return EXIT_SUCCESS;
}
