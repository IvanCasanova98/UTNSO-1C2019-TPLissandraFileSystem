#include "gossiping.h"

t_list * tabla_gossip;
pthread_mutex_t mutex_gossip;
pthread_mutex_t mutex_gossip_send;
pthread_mutex_t mutex_gossip_recv;
pthread_mutex_t mutex_gossip_edit;

void startup_tabla_gossip(t_config * config)
{
	tabla_gossip = list_create();

	char * IP = config_get_string_value(config,"IP");
	int PUERTO = config_get_int_value(config,"PUERTO");
	int NUMBER = config_get_int_value(config,"MEMORY_NUMBER");

	SEED * seed_i = malloc(sizeof(SEED));
	seed_i->IP = malloc(strlen(IP)+1);

	strcpy(seed_i->IP,IP);
	seed_i->NUMBER=NUMBER;
	seed_i->PUERTO=PUERTO;
	seed_i->ON=1;

	list_add(tabla_gossip,seed_i);

	pthread_mutex_init(&mutex_gossip,NULL);
	pthread_mutex_init(&mutex_gossip_send,NULL);
	pthread_mutex_init(&mutex_gossip_recv,NULL);
	pthread_mutex_init(&mutex_gossip_edit,NULL);
}


void gossiping(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	char** MEMORY_NUMBERS = config_get_array_value(parametro->config, "MEMORY_NUMBERS");
	char** IP_SEEDS = config_get_array_value(parametro->config, "IP_SEEDS");
	char** PUERTO_SEEDS = config_get_array_value(parametro->config, "PUERTO_SEEDS");

	int retardo_gossiping = config_get_int_value(parametro->config,"RETARDO_GOSSIPING");

	int cant_memorias = cant_elementos_array(MEMORY_NUMBERS);
	int cod_op = 9;

	while(1)
	{
		for(int i=0; i<cant_memorias; i++)
		{

			pthread_mutex_lock(&mutex_gossip);
			char ** ip_sin_comillas = string_split(IP_SEEDS[i],"\"");
			int conexion = iniciar_conexion_gossiping(ip_sin_comillas[i],PUERTO_SEEDS[i]);

			if(conexion!=-1)
			{
				send(conexion,&cod_op,sizeof(int),MSG_WAITALL);
				recibir_seed(conexion);
				enviar_memorias(conexion,parametro->config);
				terminar_conexion(conexion);
				log_info(parametro->logger,"Gossiping realizado");
			}
			else
			{
				cambiar_estado_memoria( atoi(MEMORY_NUMBERS[i]), 0);
				log_info(parametro->logger,"Gossiping abortado");
			}
			pthread_mutex_unlock(&mutex_gossip);
			sleep(retardo_gossiping);
		}
	}


}


//--------------------------AUX GOSSIPING

void agregar_memoria(SEED* nueva_memoria)
{
	list_add(tabla_gossip,nueva_memoria);
}

void reemplazar_memoria(SEED* memoria_nueva)
{
	int index = index_memory(memoria_nueva->NUMBER);
	SEED* memoria_a_reemplazar = list_get(tabla_gossip, index);
	void _eliminar_memoria(SEED* memoria){free(memoria);};
	list_replace_and_destroy_element(tabla_gossip, index, memoria_nueva, _eliminar_memoria);
}

bool table_has_memory(int numero_memoria)
{
	bool _comparar_numero(SEED * elemento){return numero_memoria == elemento->NUMBER;}
	return list_any_satisfy(tabla_gossip, _comparar_numero);
}

t_list * get_tabla_gossip()
{
	return tabla_gossip;
}

void mostrar_tabla_gossip()
{
	while(1)
	{
		list_iterate(tabla_gossip,imprimir_memoria);
		puts("\n");
		sleep(2);
	}
}

void imprimir_memoria(SEED * seed)
{
	printf("\nMemoria: %d - Puerto: %d - Ip: %s - Estado: %d", seed->NUMBER,seed->PUERTO,seed->IP,seed->ON);
}

int index_memory(int numero_memoria)//DEVUELVE EL NUMERO DE INDEX DE UNA MEMORIA EN LA LISTA DE GOSSIPING
{
	int i = 0;
	SEED* elemento = list_get(tabla_gossip, 0);
	while(numero_memoria != elemento->NUMBER)
	{
		i++;
		elemento = list_get(tabla_gossip, i);
	}
	return i;
}

void cambiar_estado_memoria(int numero_memoria, int on)
{
	bool _get_memoria(SEED* elemento){return elemento->NUMBER == numero_memoria;}
	SEED* memoria_a_editar = (SEED *) list_remove_by_condition(tabla_gossip, _get_memoria);

	if(memoria_a_editar==NULL){}
	else
	{
		memoria_a_editar->ON = on;
		list_add(tabla_gossip,memoria_a_editar);
	}
}
