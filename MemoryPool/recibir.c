#include "recibir.h"

//---------------------------LEVANTAR SERVIDOR

void* servidor(void * arg)
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg ;

	int server_fd = iniciar_servidor(parametro->config);

	int cliente_fd = esperar_cliente(server_fd);

	recibir_paquetes(cliente_fd, server_fd, parametro->config, parametro->logger);

    return NULL;
}

//----------------------------RECIBIR PAQUETE

void recibir_paquetes(int cliente_fd, int server_fd, t_config* config, t_log* logger)
{
	int cod_op;

	while(1){

		if (cliente_fd!=0)
		{
			cod_op = recibir_operacion(cliente_fd);
		}
		else
		{
			cliente_fd = esperar_cliente(server_fd);
			cod_op = recibir_operacion(cliente_fd);
		}

		switch(cod_op)
		{
		case CREATE:
			break;
		case DROP:
			break;
		case DESCRIBE:
			break;
		case SELECT:;
			t_paquete_select *paquete_select = deserializar_paquete_select(cliente_fd);

			loggear_paquete_select(paquete_select, logger);

			selectf(paquete_select, config, logger);

			break;
		case INSERT:;
			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);

			loggear_paquete_insert(paquete_insert, logger);

			insert(paquete_insert, config, logger);

			break;
		case JOURNAL:
			break;
		case RUN:
			break;
		case ADD:
			break;
		case -1:
			cliente_fd=0;
			break;
		default:
			printf("operacion desconocida");
			break;
		}
	}
}

//----------------------------TIPO DE OPERACION RECIBIDA

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0) return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

//----------------------------DESERIALIZAR PAQUETES

t_paquete_select* deserializar_paquete_select(int socket_cliente)
{
	int desplazamiento = 0;
	size_t tamanioTabla;

	recv(socket_cliente, &tamanioTabla, sizeof(int) ,MSG_WAITALL);

	t_paquete_select *paqueteSelect= malloc(tamanioTabla+sizeof(uint16_t)+sizeof(int));
	paqueteSelect->nombre_tabla = malloc(tamanioTabla);

	void *buffer = malloc(tamanioTabla+sizeof(uint16_t));

	recv(socket_cliente, buffer, tamanioTabla+sizeof(uint16_t) ,MSG_WAITALL);

	memcpy(paqueteSelect->nombre_tabla,buffer + desplazamiento, tamanioTabla);
	desplazamiento+= tamanioTabla;
	memcpy(&(paqueteSelect->valor_key),buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	paqueteSelect->nombre_tabla_long=tamanioTabla;
	free(buffer);
	return paqueteSelect;
}


t_paquete_insert* deserializar_paquete_insert(int socket_cliente)
{
	void *buffer_para_longitudes = malloc(sizeof(uint32_t)*2);

	int desplazamiento = 0;
	size_t tamanioTabla;
	size_t tamanioValue;

	recv(socket_cliente, buffer_para_longitudes, sizeof(uint32_t)*2 ,MSG_WAITALL);

	memcpy(&tamanioTabla ,buffer_para_longitudes,sizeof(uint32_t));
	memcpy(&tamanioValue ,buffer_para_longitudes+sizeof(uint32_t),sizeof(uint32_t));
	t_paquete_insert *paquete_insert = malloc(tamanioTabla + tamanioValue + sizeof(int)*2 + sizeof(uint16_t)+ sizeof(long long));

		paquete_insert->nombre_tabla = malloc(tamanioTabla);
		paquete_insert->value = malloc(tamanioValue);

	void *buffer = malloc(sizeof(long long)+ sizeof(uint16_t) +tamanioTabla+tamanioValue);

	recv(socket_cliente, buffer, sizeof(long long)+ sizeof(uint16_t) +tamanioTabla+tamanioValue ,MSG_WAITALL);

	memcpy(paquete_insert->nombre_tabla,buffer + desplazamiento,tamanioTabla);
	desplazamiento+= tamanioTabla;
	memcpy(paquete_insert->value,buffer + desplazamiento, tamanioValue);
	desplazamiento+= tamanioValue;
	memcpy(&(paquete_insert->valor_key),buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento+= sizeof(uint16_t);
	memcpy(&(paquete_insert->timestamp),buffer + desplazamiento, sizeof(long long));
	desplazamiento+= sizeof(long long);
	paquete_insert->nombre_tabla_long = tamanioTabla;
	paquete_insert->value_long = tamanioValue;
	free(buffer_para_longitudes);
	free(buffer);
	return paquete_insert;
}
