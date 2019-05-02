#include "paquete.h"

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		puts(" *** No se recibio correctamente el COD OP ***");
		return -1;
	}
}


t_paquete_select* deserializar_paquete_select(int socket_cliente){



	int desplazamiento = 0;
	void *buffer = malloc(11);
	struct t_paquete_select *paqueteSelect = malloc(11);
	recv(socket_cliente, buffer, 11 ,MSG_WAITALL);
	memcpy(&(paqueteSelect->nombre_tabla),buffer + desplazamiento,7);
	desplazamiento+= 7;
	memcpy(&(paqueteSelect->valor_key),buffer + desplazamiento, 4);
	desplazamiento+= 4;
	free(buffer);
	return paqueteSelect;
}



