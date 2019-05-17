/*
 * envio.c
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */
#include "envio.h"

t_log* iniciar_logger() {
	return log_create("Kernel.log", "Kernel", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	return config_create("Kernel.config");
}

//----------------------------SERIALIZAR PAQUETES



//----------------------------ENVIAR PAQUETES

