#include "kernel.h"

int main(void){

	t_log* logger = iniciar_logger();
	t_config* config = leer_config();

	char* ip = config_get_string_value(config, "IP_MEMORIA");
	char* puerto = config_get_string_value(config, "PUERTO_MEMORIA");

	int conexion = iniciar_conexion(logger, ip, puerto);

	pedir_seed(conexion);

	close(conexion);//desconecta de memoria 0 (A MENOS DE QUE SE VUELVA A CONECTAR A LA MEMORIA 0)

	SEED memoria = elegir_memoria();
	conexion = iniciar_conexion(logger, memoria.IP, memoria.PUERTO);//conectar a la memoria elegida

	ingresar_paquete(conexion);

	terminar_kernel(logger,config,conexion);

	return 1;
}
