#include "planificador.h"
t_queue* cola_ready;

void planificador(void * arg)
{
//	struct parametros * parametro;
//	parametro = ( struct parametros *) arg ;

	while(1)
	{
		if(elementos_en_la_cola())
		{
			if(1/*hay menos de 4 hilos*/)
			{
				crear_hilo(arg);//CREAR NUEVO HILO
				//MANDAR A EJECUCION
			}
		}

//		printf("NO HAY ELEMENTOS EN LA COLA");
	}

}

void dispatcher(void* arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	t_proceso* proceso = queue_pop(cola_ready);

	if (proceso->boolean)
	{
		t_queue* cola_lql = queue_create();
		cola_lql = proceso->elemento;
		for(int i=0; i<4; i++)//CAMBIAR 4 POR LECTURA DE CONFIG
		{
			if(queue_size(cola_lql)>0)
			{
				char* linea_request = queue_pop(cola_lql);
				parametro->parametros = linea_request;
				request(parametro);
			}
		}
		int tamanio = tamanio_lql(cola_lql);
		if(tamanio!=0)
		{
//			t_proceso* proceso_devuelto = malloc(tamanio+sizeof(int));
//			proceso_devuelto->boolean=1;
//			proceso_devuelto->elemento = cola_lql;
			queue_push(cola_ready, proceso);
		}else{
			free(proceso);
		}
	}
	else
	{
		parametro->parametros = proceso->elemento;
		request(parametro);
	}
}

//------------------------------HILOS----------------------------
void crear_hilo(void* arg)
{
	pthread_t hilo_dispatcher;
	pthread_create(&hilo_dispatcher, NULL, dispatcher, arg);
	pthread_join(hilo_dispatcher,NULL);
}


//-------------------------COLA DE READY-------------------------
//CREAR COLA
void startup_cola_ready()
{
	cola_ready = queue_create();
}

//AGREGAR A COLA
void agregar_a_cola(char* request)
{

	char ** vector_parametros = malloc(strlen(request));
	vector_parametros=string_split(request, " ");

	int cod_ingresado = codigo_ingresado(vector_parametros[0]);


	switch(cod_ingresado)
	{
	case 6:;
		t_queue* proceso_lql = queue_create();

		char * path = strcat(vector_parametros[1], ".lql");

		for(int i=0;vector_parametros[i];i++) printf("\n-%s-",vector_parametros[i]);



		string_to_lower(path);
		proceso_lql = lql_to_queue(path);

		int tamanio = tamanio_lql(proceso_lql);
		t_proceso* lql_proceso = malloc(tamanio+sizeof(int)+1);

		lql_proceso->boolean=1;
		lql_proceso->elemento = proceso_lql;
		queue_push(cola_ready,lql_proceso);
		//free(lql_proceso);
		break;
	case 8:
		printf("Op. desconocida");
		break;
	default:;
		t_proceso* request_proceso = malloc(strlen(request)+sizeof(int));
		request_proceso->boolean=0;
		request_proceso->elemento = request;
		queue_push(cola_ready,request_proceso);
		break;
	}
	free(vector_parametros);
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
