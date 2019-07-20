#include "API.h"

pthread_mutex_t mutex;

void selectf(int cliente,t_paquete_select* paquete, t_config* config, t_log* logger)
{

	if(!(condicion_select(paquete -> nombre_tabla,paquete -> valor_key)))
	{
		int conexion = iniciar_conexion(config);
		enviar_select_lissandra(conexion, paquete, logger);
		t_pagina* pagina_lissandra = deserializar_pagina(conexion);
		terminar_conexion(conexion);

		if(pagina_lissandra->timestamp!=0)
		{
			pagina_lissandra->valor_key = paquete->valor_key;
			t_pagina_completa* pagina_completa_lissandra = crear_pagina_completa(pagina_lissandra);

			if(condicion_insert(paquete->nombre_tabla))
			{
				agregar_pagina(paquete->nombre_tabla, pagina_completa_lissandra);
			}
			else
			{
				journal(config, logger);
				agregar_pagina(paquete->nombre_tabla, pagina_completa_lissandra);
			}
		}
		else
		{
			char* value_error = pagina_lissandra->value;
			size_t tamanio_value_error = strlen(value_error)+1;

			if(cliente!=NULL)
			{
				send(cliente,&tamanio_value_error,sizeof(size_t),0);
				send(cliente,value_error,tamanio_value_error,0);
				log_info(logger,"Respuesta enviada: %s\n", value_error);
			}
			else
			{
				log_error(logger,"Respuesta: %s\n", value_error);
			}


			return;
		}
	}

	t_pagina* pagina_encontrada = buscar_pagina(paquete -> nombre_tabla, paquete -> valor_key);

	int bytes = sizeof(int) + strlen(pagina_encontrada -> value) + 1;
	void* a_enviar = serializar_mensaje(pagina_encontrada -> value, bytes);
	if (cliente != NULL){
		send(cliente,a_enviar,bytes,0);
		log_info(logger,"Respuesta enviada: %s\n", pagina_encontrada -> value);
	}
	else{log_info(logger,"Respuesta: %s\n", pagina_encontrada -> value);}

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
		enviar_create_lissandra(paquete, config, logger);
		log_info(logger,"Tabla creada: %s\n", paquete->nombre_tabla);
	}

}



//--------------------------------------------------------------
void journal(t_config* config, t_log* logger)
{
	int conexion_LFS = iniciar_conexion(config);

	void _enviar_modificados(void * nombre_tabla){enviar_modificados(nombre_tabla,conexion_LFS,config,logger);}

	dictionary_iterator(tabla_segmentos, _enviar_modificados);

}

void enviar_modificados(char * nombre_tabla, int conexion_LFS, t_config* config, t_log* logger)
{
	t_list * lista_paginas_modificadas = list_create();
	lista_paginas_modificadas = paginas_modificadas(nombre_tabla);

	void _enviar(void * elemento){enviar(elemento,nombre_tabla,conexion_LFS,config, logger);}

	list_iterate(lista_paginas_modificadas,_enviar);

}

void enviar(t_pagina_completa* pagina_completa, char * nombre_tabla, int conexion_LFS,t_config* config, t_log* logger)
{
	t_paquete_insert * paquete = crear_paquete_insert(nombre_tabla,pagina_completa->pagina->valor_key,pagina_completa->pagina->value, pagina_completa->pagina->timestamp);
	enviar_insert_lissandra(paquete,config,logger);
}
