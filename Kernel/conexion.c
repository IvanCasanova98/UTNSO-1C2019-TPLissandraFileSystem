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
	{
//		printf("error");
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

int iniciar_conexion_inicial(t_log* logger, char* ip, char* puerto) //tiene que llegar logger, archivo config y numero de conexion (int)
{
	int conexion = -1;

	while(conexion == -1){
		conexion = crear_conexion(ip,puerto);
		sleep(1);
	}
//	recibir_numero_memoria(conexion);
	describe(conexion,NULL);

	return conexion;
}

int iniciar_conexion_request(t_log* logger, char* ip, char* puerto) //tiene que llegar logger, archivo config y numero de conexion (int)
{
	int conexion = crear_conexion(ip,puerto);

//	recibir_numero_memoria(conexion);
//	describe(conexion,NULL);

	return conexion;
}

int conectarse_a_memoria(char** vector_request, t_log* logger)
{
	SEED * memoria;
	int conexion_nueva;

	int cod_ingresado = codigo_ingresado(vector_request[0]);

	switch(cod_ingresado)
	{
	case 0:;
		int cons_ingresada = consistencia_ingresada(vector_request[2]);
		memoria = elegir_memoria(vector_request[1],cons_ingresada);
		if(memoria==NULL)
		{
			conexion_nueva = -1;
		}
		break;
	case 1:;
		if(existe_tabla(vector_request[1]))
		{
			char * consistencia_tabla = get_consistencia(vector_request[1]);
			int cons_ingresada = consistencia_ingresada(consistencia_tabla);
			memoria = elegir_memoria(vector_request[1],cons_ingresada);
			if(memoria==NULL)
			{
				conexion_nueva = -1;
			}
		}
		else{return -1;}
		break;
	case 2:;
		int num_random = numero_random(2);
		t_list * pool_especifico = get_pool(num_random);
		int numero_memoria = list_get(pool_especifico,0);
		memoria = get_seed_especifica(numero_memoria);
		break;
	case 3:;
		if(existe_tabla(vector_request[1]))
		{
			char * consistencia_tabla = get_consistencia(vector_request[1]);
			int cons_ingresada = consistencia_ingresada(consistencia_tabla);
			memoria = elegir_memoria(vector_request[1],cons_ingresada);
			if(memoria==NULL)
			{
				conexion_nueva = -1;
			}
		}
		else{return -1;}
		break;
	case 4:;
		if(existe_tabla(vector_request[1]))
		{
			char * consistencia_tabla = get_consistencia(vector_request[1]);
			int cons_ingresada = consistencia_ingresada(consistencia_tabla);
			memoria = elegir_memoria(vector_request[1],cons_ingresada);
			if(memoria==NULL)
			{
				conexion_nueva = -1;
			}
		}
		else{return -1;}
		break;
	case 5:
		return 0;
		break;
	case 7:
		return 0;
		break;
	default:
		return -1;
		break;
	}

	if(conexion_nueva != -1)
	{
		char * puerto_char = string_itoa(memoria->PUERTO);
		char ** ip_sin_comillas = string_split(memoria->IP,"\"");

		conexion_nueva = iniciar_conexion_request(logger, ip_sin_comillas[0], puerto_char);
	}

	printf("\nCONECTADO A MEMORIA: %d", memoria->NUMBER);

	return conexion_nueva;
}

void handshake(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	char* ip = config_get_string_value(parametro->config, "IP_MEMORIA");
	char* puerto = config_get_string_value(parametro->config, "PUERTO_MEMORIA");


//	printf("\nconexion %d",conexion);

	while(1)
	{
		int conexion = iniciar_conexion_request(parametro->logger,ip,puerto);
		pedir_seed(conexion);
		//mostrar_lista_seeds(parametro->logger);
		log_info(parametro->logger, "Handshake realizado");
		close(conexion);
		sleep(7);
	}

}

void pedir_seed(int conexion)
{
	int cod_operacion=HS;
	send(conexion, &cod_operacion, sizeof(int), 0);
	recibir_seed(conexion);
}

void terminar_kernel(t_log* logger, t_config* config)
{
	log_info(logger, "DESCONECTADO DE MEMORY POOL\n");
	log_destroy(logger);
	config_destroy(config);
}
