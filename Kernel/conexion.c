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
			return 0;
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
				conexion_nueva = 0;
			}
		}
		else
		{
			log_error(logger, "No existe la tabla");
			return -1;
		}
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
				conexion_nueva = 0;
			}
		}
		else
		{
			log_error(logger, "No existe la tabla");
			return -1;
		}
		break;
	case 4:;
		if(existe_tabla(vector_request[1]))
		{
			char * consistencia_tabla = get_consistencia(vector_request[1]);
			int cons_ingresada = consistencia_ingresada(consistencia_tabla);
			memoria = elegir_memoria(vector_request[1],cons_ingresada);
			if(memoria==NULL)
			{
				conexion_nueva = 0;
			}
		}
		else
		{
			log_error(logger, "No existe la tabla");
			return -1;
		}
		break;
	case 5:
		return 0;
		break;
	case 7:
		return 0;
		break;
	default:
		return 0;
		break;
	}

	if(conexion_nueva != 0)
	{
		char * puerto_char = string_itoa(memoria->PUERTO);
		char ** ip_sin_comillas = string_split(memoria->IP,"\"");

		conexion_nueva = iniciar_conexion_request(logger, ip_sin_comillas[0], puerto_char);

		//ACA SERIA EL CASO Q ME CONECTE, TENIENDOLA Y LA HAYAN BAJADO.
		if(conexion_nueva == -1)
		{
			memory_off(memoria);
			remover_memoria_consistencia(memoria);
			puts("Fallo conexion");
			conexion_nueva = 0;
		}

		string_iterate_lines(ip_sin_comillas,free);
		free(ip_sin_comillas);

	}

//	printf("\nCONECTADO A MEMORIA: %d", memoria->NUMBER);

	return conexion_nueva;
}

void handshake(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	char* ip = config_get_string_value(parametro->config, "IP_MEMORIA");
	char* puerto = config_get_string_value(parametro->config, "PUERTO_MEMORIA");
	int cod_operacion=8;

	while(1)
	{
		sleep(7);
		int conexion = iniciar_conexion_request(parametro->logger,ip,puerto);

		send(conexion, &cod_operacion, sizeof(int), 0);

		pthread_mutex_lock(&mutex_handshake);
		enviar_memorias(conexion,parametro->config);
		recibir_seed(conexion);
		pthread_mutex_unlock(&mutex_handshake);

		log_info(parametro->logger, "Handshake realizado");

		close(conexion);
		mostrar_lista_seeds(parametro->logger);
	}
}

void enviar_memorias(int socket_cliente, t_config* config)//, int respuesta)
{
	int cant_elementos = list_size(lista_seeds);

	send(socket_cliente,&cant_elementos,sizeof(int),MSG_WAITALL);

	int i=0;

	while(i<cant_elementos)
	{
		SEED * seed_i = list_get(lista_seeds,i);

		int tamanio_total = 4*sizeof(int) + strlen(seed_i->IP)+1;

		void * envio = serealizar_seed_completa(seed_i->NUMBER,seed_i->PUERTO,seed_i->IP, seed_i->ON, tamanio_total);
		send(socket_cliente,envio,tamanio_total,MSG_WAITALL);
		free(envio);

		i++;
	}

//	bool _apagadas(SEED * seed){return seed->ON == 0;}
	list_clean_and_destroy_elements(lista_seeds,_eliminar_seed);
}


void* serealizar_seed_completa(int memoria, int puerto, char* IP, int on, int tamanio_total)
{

	int bytes_ip = strlen(IP)+1;
	//3 size of int porque so 2 de memoria y puerto, y otro de tamanio de ip
	void* buffer = malloc(tamanio_total);

	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &bytes_ip, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(buffer + desplazamiento, IP, bytes_ip);
	desplazamiento+= bytes_ip;

	memcpy(buffer + desplazamiento, &memoria, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(buffer + desplazamiento, &puerto, sizeof(int));
	desplazamiento+= sizeof(int);

	memcpy(buffer + desplazamiento, &on, sizeof(int));
	desplazamiento+= sizeof(int);

	return buffer;

}


void eliminar_seed(SEED * seed)
{
	free(seed->IP);
	free(seed);
}


void terminar_kernel(t_log* logger, t_config* config)
{
	log_info(logger, "DESCONECTADO DE MEMORY POOL\n");
	log_destroy(logger);
	config_destroy(config);
}
