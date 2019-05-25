#include "kernel.h"

int main(void){

	t_log* logger = iniciar_logger();

	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);

	recibir_SEED(conexion);

	//desconecta de memoria 0
	//elige memoria
	//conectar a la memoria elegida

	ingresar_paquete(conexion);

	terminar_conexion(logger,config,conexion);

}
