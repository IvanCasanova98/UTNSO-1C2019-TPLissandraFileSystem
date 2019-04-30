#include "MemoryPool.h"

int main(void)
{
//	void iterator(char* value){
//		log_info(logger, value);
//	}

	void tipoPaquete(t_list* lista, int cliente_fd, op_code cod_op){
		//lista = recibir_paquete(cliente_fd);
		imprimirTipoPaquete(cod_op);
		//list_iterate(lista, (void*) iterator);
	}

	logger = log_create("MemoryPool.log", "MemoryPool", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "INICIO CONEXION. Servidor listo para recibir al Kernel");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while(1)
	{
		int cod_op = recibir_operacion(cliente_fd);
		switch(cod_op)
		{
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

			nombre_tabla(cliente_fd);
			int valorkey = valor_key(cliente_fd);
			printf("%d", valorkey);
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
	case CREATE:
		log_info(logger, "Se recibio paquete tipo: CREATE");
		break;
	case DROP:
		log_info(logger, "Se recibio paquete tipo: DROP");
		break;
	case DESCRIBE:
		log_info(logger, "Se recibio paquete tipo: DESCRIBE");
		break;
	case SELECT:
		log_info(logger, "Se recibio paquete tipo: SELECT");
		break;
	case INSERT:
		log_info(logger, "Se recibio paquete tipo: INSERT");
		break;
	case JOURNAL:
		log_info(logger, "Se recibio paquete tipo: JOURNAL");
		break;
	case RUN:
		log_info(logger, "Se recibio paquete tipo: RUN");
		break;
	case ADD:
		log_info(logger, "Se recibio paquete tipo: ADD");
		break;
	default:
		log_info(logger, "Se recibio paquete tipo: DESCONOCIDO");
		break;
	}

}
