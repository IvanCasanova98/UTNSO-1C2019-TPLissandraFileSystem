#include "API.h"

pthread_mutex_t mutex;

void selectf(int cliente,t_paquete_select* paquete, t_config* config, t_log* logger)
{
//	puts("SELECT RECIBIDO");
	if(!(condicion_select(paquete -> nombre_tabla,paquete -> valor_key)))
	{
		int conexion = iniciar_conexion(config);
		enviar_select_lissandra(conexion, paquete, logger);

		t_pagina* pagina_lissandra = deserializar_pagina(conexion);

		terminar_conexion(conexion);

		if(pagina_lissandra->timestamp!=0)
		{
			pagina_lissandra->valor_key = paquete->valor_key;

			t_paquete_insert* paquete_insert = crear_paquete_insert(paquete->nombre_tabla, pagina_lissandra->valor_key,pagina_lissandra->value, pagina_lissandra->timestamp);

			insert(paquete_insert,config,logger, 0);

			if(!existe_tabla_paginas(paquete->nombre_tabla))
			{
				if (cliente != NULL){
					int bytes = sizeof(int) + strlen(pagina_lissandra -> value) + 1;
					void* a_enviar = serializar_mensaje(pagina_lissandra -> value, bytes);
					send(cliente,a_enviar,bytes,0);
					free(a_enviar);
					log_info(logger,"Respuesta enviada: %s\n", pagina_lissandra -> value);
				}
				else{log_info(logger,"Respuesta: %s\n", pagina_lissandra -> value);}

				free(paquete_insert->nombre_tabla);
				free(paquete_insert->value);
				free(paquete_insert);

				free(pagina_lissandra->value);
				free(pagina_lissandra);
				return;
			}
			free(paquete_insert->nombre_tabla);
			free(paquete_insert->value);
			free(paquete_insert);

			free(pagina_lissandra->value);
			free(pagina_lissandra);

		}
		else
		{
//			CHAR* VALUE_ERROR = STRING_NEW();
//			SIZE_T TAMANIO_VALUE_ERROR = STRLEN(PAGINA_LISSANDRA->VALUE)+1;
//			MEMCPY(VALUE_ERROR,PAGINA_LISSANDRA->VALUE,TAMANIO_VALUE_ERROR);

			size_t tamanio_value_error = strlen(pagina_lissandra->value)+1;
			char* value_error = malloc(strlen(pagina_lissandra->value)+1);
			memcpy(value_error,pagina_lissandra->value,tamanio_value_error);

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

			free(pagina_lissandra->value);
			free(pagina_lissandra);
			return;
		}
	}
	t_pagina* pagina_encontrada = buscar_pagina(paquete -> nombre_tabla, paquete -> valor_key);

	int bytes = sizeof(int) + strlen(pagina_encontrada -> value) + 1;
	void* a_enviar = serializar_mensaje(pagina_encontrada -> value, bytes);
	if (cliente != NULL){
		send(cliente,a_enviar,bytes,0);
		free(a_enviar);
		log_info(logger,"Respuesta enviada: %s\n", pagina_encontrada -> value);
	}
	else{log_info(logger,"Respuesta: %s\n", pagina_encontrada -> value);}

}

void insert(t_paquete_insert* paquete, t_config* config, t_log* logger, int flag_modificado)
{
	t_pagina* pagina = crear_pagina(paquete -> valor_key, paquete -> value, paquete -> timestamp);
		t_pagina_completa* pagina_completa = crear_pagina_completa(pagina);

		if(condicion_insert(paquete,config))
		{
			if(verificar_espacio_memoria())
			{
				if(existe_tabla_paginas(paquete->nombre_tabla))
				{
					if(existe_pagina(paquete->nombre_tabla,paquete->valor_key)) //REMPLAZA KEY IGUAL A UNA
					{
						pagina_completa->flag = flag_modificado; // 1
						remplazo_especifico(paquete->nombre_tabla,pagina_completa,config);
						log_info(logger,"Pagina remplazada en: %s\n", paquete->nombre_tabla);


					}
					else //CASO BASE TENGO LUGAR Y KEY NUEVA
					{
						pagina_completa->flag=flag_modificado;
						agregar_pagina(paquete -> nombre_tabla, pagina_completa);
						log_info(logger,"Pagina agregada en: %s\n", paquete->nombre_tabla);

					}
				}
				else
				{
					crear_tabla_paginas(paquete->nombre_tabla, "EC", NULL);
					insert(paquete, config, logger, flag_modificado);
					free(pagina->value);
					free(pagina);
					free(pagina_completa);
				}
			}
			else
			{
				if(existe_pagina(paquete->nombre_tabla,paquete->valor_key)) //REMPLAZA KEY IGUAL A UNA
				{
					pagina_completa->flag = flag_modificado; // 1
					remplazo_especifico(paquete->nombre_tabla,pagina_completa,config);
					log_info(logger,"Pagina remplazada en: %s\n", paquete->nombre_tabla);


				}
				else if(puede_reemplazar(paquete->nombre_tabla)) //LRU
					{
						pagina_completa->flag = flag_modificado;
						reemplazar_pagina(paquete->nombre_tabla, pagina_completa, config);
						log_info(logger,"Pagina reemplazada en: %s\n", paquete->nombre_tabla);

					}
					else//MEMORIA EN ESTADO FULL, no hay ninguna con flag 0
					{
						journal(config,logger);
						pagina_completa -> flag = flag_modificado;
						agregar_pagina(paquete -> nombre_tabla, pagina_completa);
						log_info(logger,"Pagina agregada en: %s\n", paquete->nombre_tabla);

					}
			}
		}
		else
		{
			log_error(logger,"Value invalido");
		}
}

void create(t_paquete_create* paquete, t_config* config, t_log* logger)
{
	puts("CREATE RECIBIDO");
	int boolean;
	if(existe_tabla_paginas(paquete -> nombre_tabla))
	{
		log_error(logger,"Ya existe la tabla: %s\n", paquete->nombre_tabla);
	}
	else
	{
		crear_tabla_paginas(paquete->nombre_tabla,paquete->consistencia,paquete->particiones);
		log_info(logger,"Tabla creada: %s\n", paquete->nombre_tabla);
		enviar_create_lissandra(paquete, config, logger);
	}
}


void drop(char * nombre_tabla, t_config* config, t_log* logger)
{
	if(existe_tabla_paginas(nombre_tabla))
	{
		eliminar_segmento(nombre_tabla);
		enviar_drop_lissandra(nombre_tabla,config,logger);
		log_info(logger, "Tabla %s eliminada de memoria", nombre_tabla);
		mostrar_tabla_segmentos();
	}
	else
	{
		log_error(logger,"No existe dicha tabla");
	}
}

//void describe(char* parametros_paquete, t_log* logger, t_config* config)
//{
//	t_paquete_describe_lfs* paquete_describe_lfs = paquete_describe_para_lfs(parametros_paquete,logger);
//	enviar_describe_lissandra(paquete_describe_lfs,config,logger);
//}

///////////////////////////////////////////////////
//-------------------------------------JOURNAL-----
///////////////////////////////////////////////////


//AGREGAR QUE BAJE EL ESPACIO DISPONIBLE X INSERT CON JOURNAL
void journal(t_config* config, t_log* logger)
{

	log_info(logger, "COMENZO JOURNAL");

	int conexion_LFS = iniciar_conexion(config);

	void _enviar_modificados(void * nombre_tabla, void * lista){enviar_modificados(nombre_tabla,lista,conexion_LFS,config,logger);}

	pthread_mutex_lock(&mutex);
	dictionary_iterator(tabla_segmentos, _enviar_modificados);
	pthread_mutex_unlock(&mutex);

//	mostrar_tabla_segmentos();
//	mostrar_tamanio_memoria_ocupada();
	log_info(logger, "FINALIZO JOURNAL");
}



void time_journal(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	int time = config_get_int_value(parametro->config,"RETARDO_JOURNAL");

	while(1)
	{
		usleep(time);
		journal(parametro->config, parametro->logger);
	}
}

//-------------------UTILS JOURNAL

void enviar(t_pagina_completa* pagina_completa, char * nombre_tabla, int conexion_LFS,t_config* config, t_log* logger)
{
	t_paquete_insert * paquete = crear_paquete_insert(nombre_tabla,pagina_completa->pagina->valor_key,pagina_completa->pagina->value, pagina_completa->pagina->timestamp);
	enviar_insert_lissandra(paquete,config,logger);
}

void enviar_modificados(char * nombre_tabla, t_list* lista ,int conexion_LFS, t_config* config, t_log* logger)
{
	t_list* lista_paginas_modificadas = paginas_modificadas(lista);

	if (!list_is_empty(lista_paginas_modificadas))
	{
		int i = 0;

		void _enviar(void * elemento)
		{
			enviar(elemento,nombre_tabla,conexion_LFS,config, logger);
		}

		list_iterate(lista_paginas_modificadas,_enviar);

		list_destroy_and_destroy_elements(lista_paginas_modificadas,eliminar_pagina_completa);
	}
}

void eliminar_pagina_completa(void * pagina_completa)
{
	free(((t_pagina_completa *) pagina_completa)->pagina->value);
	free(((t_pagina_completa *) pagina_completa)->pagina);
	free(((t_pagina_completa *) pagina_completa));
}



//-----------------------------SEMAFOROS

void startup_semaforos()
{
	pthread_mutex_init(&mutex,NULL);
}
