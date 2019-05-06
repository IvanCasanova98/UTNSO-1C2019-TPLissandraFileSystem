#include "MemoryPool.h"

int main(void)
{
	logger = iniciar_logger();
	t_config* config = leer_config();

	int server_fd = iniciar_servidor(config);

	int cliente_fd = esperar_cliente(server_fd);

	recibir_paquetes(logger, cliente_fd);

	return EXIT_SUCCESS;

}






