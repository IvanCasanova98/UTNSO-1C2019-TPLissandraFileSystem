#include "API.h"

void selectf(int cliente,t_paquete_select* paquete, t_config* config, t_log* logger)
{
	if(condicion_select(paquete -> nombre_tabla,paquete -> valor_key))
	{
		t_pagina* pagina_encontrada = buscar_pagina(paquete -> nombre_tabla, paquete -> valor_key);

		int bytes = sizeof(int) + strlen(pagina_encontrada -> value) + 1;
		void* a_enviar = serializar_mensaje(pagina_encontrada -> value, bytes);
		send(cliente,a_enviar,bytes,0);
	}
	else
	{
		enviar_select_error(cliente); //AGREGADO
		//enviar_select_lissandra(paquete, config, logger);
	}
}

void insert(t_paquete_insert* paquete, t_config* config, t_log* logger)
{
	if(condicion_insert(paquete -> nombre_tabla))
	{
		t_pagina* pagina = crear_pagina(paquete -> valor_key, paquete -> value, paquete -> timestamp);
		t_pagina_completa* pagina_completa = crear_pagina_completa(pagina);
		agregar_pagina(paquete -> nombre_tabla, pagina_completa);
	}
	else
	{
		printf("No hay mas lugar");
		//enviar_insert_lissandra(paquete, config, logger);
	}
}

void create(t_paquete_create* paquete, t_config* config, t_log* logger)
{
	if(existe_tabla_paginas(paquete -> nombre_tabla))
	{
		printf("Ya existe la pagina");
	}
	else
	{
		crear_tabla_paginas(paquete->nombre_tabla, paquete->particiones);
	}
}
