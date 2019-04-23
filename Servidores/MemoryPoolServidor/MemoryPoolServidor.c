#include "MemoryPoolServidor.h"

int main(void)
{
	void iterator(char* value){
		printf("%s\n", value);
		log_info(logger, value);
	}

	void tipoPaquete(t_list* lista, int cliente_fd, op_code cod_op){
		lista = recibir_paquete(cliente_fd);
		imprimirTipoPaquete(cod_op);
		list_iterate(lista, (void*) iterator);
	}


	logger = log_create("MemoryPoolServidor.log", "MemoryPoolServidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "INICIO CONEXION. Servidor listo para recibir al Kernel");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while(1)
	{
		int cod_op = recibir_operacion(cliente_fd);
		switch(cod_op)
		{
		case PAQUETE:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case CREATE:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case DROP:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case DESCRIBE:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case SELECT:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case INSERT:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case JOURNAL:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case RUN:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case ADD:
			tipoPaquete(lista,cliente_fd,cod_op);
			break;
		case -1:
			log_error(logger, "FIN CONEXION. Kernel desconectado. Terminando Servidor Memory Pool.");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida.");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void imprimirTipoPaquete(op_code cod_op){
	switch(cod_op){
	case PAQUETE:
		printf("Paquete tipo PAQUETE:\n");
		break;
	case CREATE:
		printf("Paquete tipo CREATE:\n");
		break;
	case DROP:
		printf("Paquete tipo DROP:\n");
		break;
	case DESCRIBE:
		printf("Paquete tipo DESCRIBE:\n");
		break;
	case SELECT:
		printf("Paquete tipo SELECT:\n");
		break;
	case INSERT:
		printf("Paquete tipo INSERT:\n");
		break;
	case JOURNAL:
		printf("Paquete tipo JOURNAL:\n");
		break;
	case RUN:
		printf("Paquete tipo RUN:\n");
		break;
	case ADD:
		printf("Paquete tipo ADD:\n");
		break;
	default:
		printf("Tipo desconocido.");
		break;
	}

}
