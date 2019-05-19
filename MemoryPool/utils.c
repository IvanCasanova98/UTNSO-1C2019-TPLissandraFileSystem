#include "utils.h"

//----------------------------ARCHIVOS LOGGER Y CONFIG

t_log* iniciar_logger()
{
	return log_create("MemoryPool.log", "MemoryPool", 0, LOG_LEVEL_INFO);
}

t_config* leer_config(char* archivo)
{
	return config_create(archivo);
}

//----------------------------------------------------LOGGEO DE PAQUETES

void loggear_paquete_select(t_paquete_select* paquete, t_log* logger)
{
	log_info(logger, "NUEVO PAQUETE SELECT CREADO\nNombre tabla: %s\nValor KEY   : %d", paquete->nombre_tabla,paquete->valor_key);
}

void loggear_paquete_insert(t_paquete_insert* paquete, t_log* logger)
{
	log_info(logger, "NUEVO PAQUETE INSERT CREADO\nNombre tabla: %s\nValor KEY   : %d\nValue       : %s", paquete->nombre_tabla, paquete->valor_key, paquete->value);
}
