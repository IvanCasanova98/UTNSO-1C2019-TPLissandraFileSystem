 #include "conexion.h"

int crear_conexion(char* ip, char* puerto)
{
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

int iniciar_conexion(t_log* logger, char* ip, char* puerto) //tiene que llegar logger, archivo config y numero de conexion (int)
{
	log_info(logger, "CONECTANDO CON MEMORY POOL");

	int conexion = crear_conexion(ip,puerto);

	describe(conexion,NULL);

	return conexion;
}

void pedir_seed(int conexion)
{
	printf("\nESPERANDO MEMORIAS");
	int cod_operacion=HS;
	send(conexion, &cod_operacion, sizeof(int), 0);
	recibir_seed(conexion);
	printf("\nMEMORIAS RECIBIDAS");
}

void terminar_kernel(t_log* logger, t_config* config, int conexion)
{
	log_info(logger, "DESCONECTADO DE MEMORY POOL\n");
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}
