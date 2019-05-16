#include "recibir.h"

//----------------------------RECIBIR PAQUETE
void* recibir_paquetes(void *arg)
{
	int cliente_fd = *((int *) arg);
	free(arg);


//	int cliente_fd=((struct argumentosEnvioPaquete*)argumentos)->clientefd;
//	int server_fd= ((struct argumentosEnvioPaquete*)argumentos)->serverfd;
//	int *cliente_fd = (int *)clienteFd;
//	int *ptr_cliente_fd = (int *)clienteFd;
//	int cliente_fd = *ptr_cliente_fd;
//	int cliente_fd = (int)clienteFd;
//	int cliente_fd = (int) arg;

	printf("\n\n\nEL hilo entro a la funcion recibir_paquetes!!!\n\n\n");
	int cod_op = recibir_operacion(cliente_fd);

		if (cliente_fd==0){
			printf("Error al conectarse para intentar recibir paquete\ņ");
			pthread_exit(NULL);
		}

		switch(cod_op){
		case CREATE:
			log_info(logger, "Se recibio paquete tipo: CREATE");
			break;
		case DROP:

			log_info(logger, "Se recibio paquete tipo: DROP");
			break;

		case DESCRIBE:
			log_info(logger, "Se recibio paquete tipo: DESCRIBE");
			break;
		case SELECT: ;
			system("clear");
			t_paquete_select *paquete_select=deserializar_paquete_select(cliente_fd);
			log_info(logger, "SELECT %s %d ",paquete_select->nombre_tabla, paquete_select->valor_key);
			free(paquete_select->nombre_tabla);
			free(paquete_select);
			break;
		case INSERT: ;
			system("clear");
			t_paquete_insert* paquete_insert = deserializar_paquete_insert(cliente_fd);
			log_info(logger, "INSERT %s %d %s %d ",paquete_insert->nombre_tabla, paquete_insert->valor_key,paquete_insert->value, paquete_insert->timestamp);


			free(paquete_insert->value);
			free(paquete_insert->nombre_tabla);
			free(paquete_insert);
			break;
		case JOURNAL:

			//AGREGE ACA PARA SALIR. !!
			log_info(logger, "FIN CONEXION.\n");
			cliente_fd=0;
			log_info(logger, "Se recibio paquete tipo: JOURNAL");
			pthread_exit(NULL);

			break;
		case RUN:
			log_info(logger, "Se recibio paquete tipo: RUN");
			break;
		case ADD:
			log_info(logger, "Se recibio paquete tipo: ADD");
			break;
		case -1:
			log_info(logger, "FIN CONEXION.\n");
			cliente_fd=0;
			pthread_exit(NULL);
			break;
		default:
			log_warning(logger, "Operacion desconocida.");
			break;
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
		puts(" *** No se recibio correctamente el COD OP ***");
		return -1;
	}
}


//----------------------------DESERIALIZAR PAQUETES
t_paquete_select* deserializar_paquete_select(int socket_cliente){


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


t_paquete_insert* deserializar_paquete_insert(int socket_cliente){

	void *buffer_para_longitudes = malloc(sizeof(uint32_t)*2); //hice este buffer para sacar los tamaños de las longitudes variables. EL otro buffer saca TODa la info deaa

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

