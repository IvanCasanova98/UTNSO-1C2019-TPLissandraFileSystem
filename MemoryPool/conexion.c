#include "conexion.h"

//---------------------------LEVANTAR SERVIDOR

void* servidor(void *arg){

	int server_fd = iniciar_servidor();

	int cliente_fd = esperar_cliente(server_fd);

	recibir_paquetes(cliente_fd, server_fd);

    return NULL;
}

//----------------------------CONEXION COMO SERVIDOR
int iniciar_servidor()
{


	t_config* config = leer_config();
	int socket_servidor;
	char* ip = config_get_string_value(config, "IP");
	char* puerto = config_get_string_value(config, "PUERTOSERVER");

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

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{

	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	return socket_cliente;
}

//----------------------------CONEXION COMO CLIENTE

int crear_conexion(char *ip, char* puerto){
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

int iniciar_conexion(t_log* logger, t_config* config){ //tiene que llegar logger, archivo config y numero de conexion (int)

	int conexion = crear_conexion(
		config_get_string_value(config, "IP"),
		config_get_string_value(config, "PUERTOLFS")
	);
	return conexion;
}

void terminar_conexion(t_log* logger, t_config* config, int conexion){
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}
