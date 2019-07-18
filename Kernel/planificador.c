#include "planificador.h"

t_queue* cola_ready;

int elementos_ready;
int hilos_ejecutando;

void planificador(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	char * hilos = config_get_string_value(parametro->config, "MULTIPROCESAMIENTO");
	int hilos_disponibles = atoi(hilos);

	while(1)
	{
		if(elementos_ready>0)
		{
			if(hilos_ejecutando<hilos_disponibles)
			{
				elementos_ready--;
				crear_hilo(arg);
				hilos_ejecutando++;
			}
		}
	}
}

void dispatcher(void* arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	char * valor_quantum = config_get_string_value(parametro->config,"QUANTUM");
	int quantum = atoi(valor_quantum);


	t_proceso* proceso = queue_pop(cola_ready);

	if (proceso->boolean)
	{
		for(int i=0; i<quantum; i++)
		{
			if(queue_size(proceso->elemento)>0)
			{
				char* linea_request = queue_pop(proceso->elemento);
				parametro->parametros = linea_request;
				request(parametro);
				free(linea_request);
			}
		}
		int tamanio = tamanio_lql(proceso->elemento);
		if(tamanio!=0)
		{
			queue_push(cola_ready, proceso);
			elementos_ready++;
		}else{
			free(proceso);
		}
	}
	else
	{
		parametro->parametros = proceso->elemento;
		request(parametro);
	}
	hilos_ejecutando--;
}

//------------------------------HILOS----------------------------
void crear_hilo(void* arg)
{
	pthread_t hilo_dispatcher;
	pthread_attr_t atributo;

	pthread_attr_init(&atributo);
	pthread_attr_setdetachstate(&atributo,PTHREAD_CREATE_DETACHED);

	pthread_create(&hilo_dispatcher, &atributo, dispatcher, arg);

	pthread_attr_destroy(&atributo);
}


//-------------------------COLA DE READY-------------------------
//CREAR COLA
void startup_cola_ready()
{
	cola_ready = queue_create();
	elementos_ready=0;
	hilos_ejecutando=0;
}

//AGREGAR A COLA
void agregar_a_cola(char* request)
{

	char ** vector_parametros;
	vector_parametros=string_split(request, " ");

	int cod_ingresado = codigo_ingresado(vector_parametros[0]);


	switch(cod_ingresado)
	{
	case 6:;
		t_queue* proceso_lql = queue_create();

		char * path = strcat(vector_parametros[1], ".lql");

		string_to_lower(path);
		proceso_lql = lql_to_queue(path);

		int tamanio = tamanio_lql(proceso_lql);
		t_proceso* lql_proceso = malloc(tamanio+sizeof(int)+1);

		lql_proceso->boolean=1;
		lql_proceso->elemento = proceso_lql;
		queue_push(cola_ready,lql_proceso);
		elementos_ready++;
		break;
	case 8:
		puts("Operacion desconocida.");
		break;
	default:;
		t_proceso* request_proceso = malloc(strlen(request)+sizeof(int)+1);
		request_proceso->boolean=0;
		request_proceso->elemento = request;

		queue_push(cola_ready,request_proceso);
		elementos_ready++;
		break;
	}

}

int tamanio(char ** vector){
	int i=0;
	while(vector[i]!=NULL)
	{
		printf("\n--%s",vector[i]);
		i++;
	}
	return i;
}

//EXTRAER DE LA COLA
void* pop_elemento()
{
	void* elemento = queue_pop(cola_ready);
	return elemento;
}

//HAY ELEMENTOS EN LA COLA
bool elementos_en_la_cola()
{
	return list_size(cola_ready->elements)>0;
}
