#include "Lissandra.h"

int main(void)
{


	logger = log_create("LissandraFileSystem.log", "LissandraFileSystem", 1, LOG_LEVEL_DEBUG);

	t_config* configLissandra = leer_config();


	int server_fd = iniciar_servidor(
			config_get_string_value(configLissandra, "IP"),
			config_get_string_value(configLissandra, "PUERTO")
			);

	log_info(logger, "INICIO CONEXION. Servidor listo para recibir una Memoria ");
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
			loggear_paquete_select(paquete_select);
			free(paquete_select);
			break;
		case INSERT: ;
			system("clear");
			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);
			loggear_paquete_insert(paquete_insert);
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




