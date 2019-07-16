#include "conexion.h"

//----------------------------ARCHIVOS LOGGER/CONFIG
t_log* iniciar_logger() {
	return log_create("Lissandra.log", "LFS", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	return config_create("Lissandra.config");
}

//----------------------------HILO PARA CONEXIONES
void* hiloEscucha(){
	int socket_servidor;
			t_config* config = leer_config();

				char* ip = config_get_string_value(config, "IP");
				char* puerto = config_get_string_value(config, "PUERTOESCUCHA");

			    struct addrinfo hints, *servinfo, *p;

			    memset(&hints, 0, sizeof(hints));
			    hints.ai_family = AF_UNSPEC;
			    hints.ai_socktype = SOCK_STREAM;
			    hints.ai_flags = AI_PASSIVE;

			    getaddrinfo(ip, puerto, &hints, &servinfo);

			    for (p=servinfo; p != NULL; p = p->ai_next)
			    {
			        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			            continue;

			        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
			            close(socket_servidor);
			            continue;
			        }
			        break;
			    }

				listen(socket_servidor, 5);

			    freeaddrinfo(servinfo);

				log_info(logger, "ESPERANDO MEMORIAS");


				struct sockaddr_in dir_cliente;
					int tam_direccion = sizeof(struct sockaddr_in);

					int socket_cliente;

					log_info(logger, "INICIO CONEXION");
				// ----------------------------------------------------------------------------


			    while ((socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion)))

				{
	//				printf("NUEVA CONEXION ACEPTADA.\n");

					pthread_t recibidorPaquetes;
					int *arg = malloc(sizeof(*arg));
					*arg = socket_cliente;

					//chekearDumpeo()
					pthread_create(&recibidorPaquetes,NULL,recibir_paquetes,arg);

				}



}

//----------------------------CONEXION
int iniciar_servidor()
{
	int socket_servidor;
	t_config* config = leer_config();
	char* ip = config_get_string_value(config, "IP");
	char* puerto = config_get_string_value(config, "PUERTOESCUCHA");

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

	log_info(logger, "ESPERANDO MEMORIA");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{

	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	log_info(logger, "INICIO CONEXION");

	return socket_cliente;
}

