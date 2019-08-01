#include "gossiping.h"

t_list * tabla_gossip;

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

	list_add(tabla_gossip,seed_i);
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
			char ** ip_sin_comillas = string_split(IP_SEEDS[i],"\"");

			int conexion = iniciar_conexion_gossiping(ip_sin_comillas[0],PUERTO_SEEDS[i]);

			if(conexion>0)
			{
				send(conexion,&cod_op,sizeof(int),MSG_WAITALL);
				enviar_memorias(conexion,parametro->config);
				recibir_seed(conexion);
				terminar_conexion(conexion);
				log_info(parametro->logger,"Gossiping realizado");
			}
		}
		sleep(retardo_gossiping);
	}

}


//--------------------------AUX GOSSIPING

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
		sleep(8);
		list_iterate(tabla_gossip,imprimir_memoria);
	}
}

void imprimir_memoria(SEED * seed)
{
	printf("\nMemoria: %d - Puerto: %d - Ip: %s", seed->NUMBER,seed->PUERTO,seed->IP);
}

