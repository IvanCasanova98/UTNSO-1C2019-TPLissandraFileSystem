#include "API.h"

pthread_mutex_t mutex;

void selectf(int cliente,t_paquete_select* paquete, t_config* config, t_log* logger)
{

	if(condicion_select(paquete -> nombre_tabla,paquete -> valor_key))
	{
		t_pagina* pagina_encontrada = buscar_pagina(paquete -> nombre_tabla, paquete -> valor_key);

		int bytes = sizeof(int) + strlen(pagina_encontrada -> value) + 1;
		void* a_enviar = serializar_mensaje(pagina_encontrada -> value, bytes);
		if (cliente != NULL){
			send(cliente,a_enviar,bytes,0);
			log_info(logger,"Respuesta enviada: %s\n", pagina_encontrada -> value);
		}
		else{log_info(logger,"Respuesta: %s\n", pagina_encontrada -> value);}
	}
	else
	{
//		enviar_select_lissandra(paquete, config, logger); //FALTA ESPERAR RESPUESTA
		log_error(logger,"PAGINA NO ENCONTRADA\n"); //SACAR CUANDO SE ENVIE A LISSANDRA
		if (cliente != NULL){enviar_select_error(cliente);} //AGREGADO
		else{} //CASO CONSOLA MP
	}


}

void insert(t_paquete_insert* paquete, t_config* config, t_log* logger)
{
	t_pagina* pagina = crear_pagina(paquete -> valor_key, paquete -> value, paquete -> timestamp);
	t_pagina_completa* pagina_completa = crear_pagina_completa(pagina);

	if(existe_tabla_paginas(paquete->nombre_tabla))
	{
		if(condicion_insert(paquete->nombre_tabla))
		{
			pagina_completa->flag=1;
			agregar_pagina(paquete -> nombre_tabla, pagina_completa);
			log_info(logger,"Pagina agregada en: %s\n", paquete->nombre_tabla);

		}else if(puede_reemplazar(paquete -> nombre_tabla))
		{
			pagina_completa->flag=1;
			reemplazar_pagina(paquete->nombre_tabla, pagina_completa);
			log_info(logger,"Pagina reemplazada en: %s\n", paquete->nombre_tabla);
		}
		else
		{
			log_error(logger,"NO HAY MAS LUGAR EN %s\n", paquete->nombre_tabla);
		}
	}
	else
	{
		log_error(logger,"NO EXISTE LA TABLA\n");

	}
}

void create(t_paquete_create* paquete, t_config* config, t_log* logger)
{
	if(existe_tabla_paginas(paquete -> nombre_tabla))
	{
		log_error(logger,"Ya existe la tabla: %s\n", paquete->nombre_tabla);
	}
	else
	{
		crear_tabla_paginas(paquete->nombre_tabla,paquete->consistencia,paquete->particiones);
		log_info(logger,"Tabla creada: %s\n", paquete->nombre_tabla);
	}

}

