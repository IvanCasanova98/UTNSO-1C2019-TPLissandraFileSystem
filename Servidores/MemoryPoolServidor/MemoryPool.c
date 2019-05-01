#include "MemoryPool.h"

int main(void)
{

	logger = log_create("MemoryPool.log", "MemoryPool", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "INICIO CONEXION. Servidor listo para recibir al Kernel");
	int cliente_fd = esperar_cliente(server_fd);

	t_paquete_select* paquete_select;

	while(1)
	{

		int cod_op = recibir_operacion(cliente_fd);
		imprimirTipoPaquete(cod_op);


		switch(cod_op){
		case CREATE:

			break;
		case DROP:

			break;
		case DESCRIBE:

			break;
		case SELECT:
			printf("Entro a select.\n");
			deserializar_paquete_select(cliente_fd,&paquete_select);
			printf("Salio de serializar.\n");
			printf("Nombre de tabla recibido: %s\n",paquete_select->nombre_tabla);
			printf("Key Value recibido: %d\n",paquete_select->valor_key);
			free(paquete_select);//va este free???
			break;
		case INSERT:

			break;
		case JOURNAL:

			break;
		case RUN:

			break;
		case ADD:

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
