#include "LissandraServidor.h"

int main(void)
{
	void iterator(char* value)
	{
		printf("%s\n", value);
		log_info(logger, value);
	}

	logger = log_create("LissandraServidor.log", "LissandraServidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "INICIO CONEXION. Servidor listo para recibir al Memory Pool");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while(1)
	{
		int cod_op = recibir_operacion(cliente_fd);
		switch(cod_op)
		{
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			printf("Se recibieron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "FIN CONEXION. Memory Pool desconectado. Terminando Servidor Lissandra.");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida.");
			break;
		}
	}
	return EXIT_SUCCESS;
}
