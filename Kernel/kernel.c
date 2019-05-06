#include "kernel.h"

int main(void){

	t_log* logger = iniciar_logger();

	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);

	ingresar_paquete(conexion, logger);

	terminar_conexion(logger,config,conexion);

}
