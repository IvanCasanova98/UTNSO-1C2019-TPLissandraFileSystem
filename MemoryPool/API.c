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

	//enviar_paquete_select(paquete, conexion);

	//espere respuesta
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
