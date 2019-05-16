#include "API.h"

void enviar_select_lissandra(t_paquete_select* paquete){

	t_log* logger = iniciar_logger();
	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);
	int cod = 3;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_select(paquete, conexion);}
	free(paquete);

	terminar_conexion(logger,config,conexion);

}

void enviar_insert_lissandra(t_paquete_insert* paquete){

	t_log* logger = iniciar_logger();
	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);
	int cod = 4;
	if (send(conexion, &cod, sizeof(int), 0) <= 0)
		puts("Error en envio de CODIGO DE OPERACION.");
	else{enviar_paquete_insert(paquete, conexion);}
	free(paquete);


	terminar_conexion(logger,config,conexion);

}

void selectf(t_paquete_select* paquete ){//enviar a otro archivo mas apropiado. debe recibir tabla y key

	//Buscar si tengo la tabla
	//if(la tengo){devolver value}
	//else {mandar a lissandra}

	if(0){
		//devolver value a kernel
	}
	else{
		enviar_select_lissandra(paquete);
	}
}

void insert(t_paquete_insert* paquete){

	if(0){

	}else{
		enviar_insert_lissandra(paquete);

	}

}
