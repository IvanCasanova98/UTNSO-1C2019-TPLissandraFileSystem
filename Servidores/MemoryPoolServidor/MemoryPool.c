#include "MemoryPool.h"

int main(void)
{


	logger = log_create("MemoryPool.log", "MemoryPool", 1, LOG_LEVEL_DEBUG);
	int server_fd = iniciar_servidor();
	log_info(logger, "INICIO CONEXION. Servidor listo para recibir al Kernel ");
	int cliente_fd = esperar_cliente(server_fd);



	while(TRUE){ //lo dejo en true porque el Kernel se da cuenta si mandan un cod_op erroneo. En caso de mandar un cod op mal, kernel se cierra

		int cod_op = recibir_operacion(cliente_fd);
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
		case SELECT: ;
			system("clear");
			t_paquete_select *paquete_select=deserializar_paquete_select(cliente_fd);
			log_info(logger, "Se recibio SELECT %s %d \n",paquete_select->nombre_tabla, paquete_select->valor_key);
			free(paquete_select);
			break;
		case INSERT: ;
			system("clear");
			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);
			log_info(logger, "Se recibio paquete tipo: INSERT %s %d %s %d \n",paquete_insert->nombre_tabla, paquete_insert->valor_key,paquete_insert->value, paquete_insert->timestamp);
			free(paquete_insert);
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




