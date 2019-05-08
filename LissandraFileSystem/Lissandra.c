#include "Lissandra.h"

int main(void)
{
	pthread_t consola;
	logger = iniciar_logger();
	t_config* config = leer_config();


	pthread_create(&consola,NULL,prenderConsola,NULL);

	int server_fd = iniciar_servidor(config);

	int cliente_fd = esperar_cliente(server_fd);

	recibir_paquetes(logger, cliente_fd, server_fd);

	log_info(logger, "FIN CONEXION");

	return EXIT_SUCCESS;

}
