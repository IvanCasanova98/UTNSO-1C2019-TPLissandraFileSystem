#include "API.h"

void selectf(t_paquete_select* paquete, t_config* config, t_log* logger)
{
	//Buscar si tengo la tabla
	//if(la tengo){devolver value}
	//else {mandar a lissandra}

	if(0)
	{
		//devolver value a kernel
	}
	else
	{
		enviar_select_lissandra(paquete, config, logger);
	}
}

void insert(t_paquete_insert* paquete, t_config* config, t_log* logger)
{
	if(0){
	}
	else
	{
		enviar_insert_lissandra(paquete, config, logger);
	}
}
