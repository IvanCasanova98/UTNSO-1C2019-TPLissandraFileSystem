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
	log_info(logger, "CONECTANDO A LA MEMORIA");

	int conexion = crear_conexion(ip,puerto);

	describe(conexion,NULL);


	return conexion;
}

int conectarse_a_memoria(char** vector_request, t_log* logger)
{
	SEED * memoria;

	if(strcmp(vector_request[0],"CREATE"))
	{
		char * consistencia_tabla = get_consistencia(vector_request[1]);
		printf("\nCONSISTENCIA PARA MEMORIA %s: ",consistencia_tabla);
		memoria = elegir_memoria(consistencia_tabla);
	}
	else
	{
		printf("\nCONSISTENCIA PARA MEMORIA con create%s: ",vector_request[2]);
		memoria = elegir_memoria(vector_request[2]);
	}

	char * puerto_char = string_itoa(memoria->PUERTO);
	char ** ip_sin_comillas = string_split(memoria->IP,"\"");

	int conexion_nueva = iniciar_conexion(logger, ip_sin_comillas[0], puerto_char);

	return conexion_nueva;
}

void pedir_seed(int conexion)
{
	int cod_operacion=HS;
	send(conexion, &cod_operacion, sizeof(int), 0);
	recibir_seed(conexion);
}

void terminar_kernel(t_log* logger, t_config* config, int conexion)
{
	log_info(logger, "DESCONECTADO DE MEMORY POOL\n");
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}
