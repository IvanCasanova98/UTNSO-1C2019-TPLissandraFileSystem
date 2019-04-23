#include "KernelCliente.h"


int main(void){

	t_log* logger = iniciar_logger();
	log_info(logger, "CONECTANDO CON MEMORY POOL");

	t_config* config = leer_config();

	int conexion = crear_conexion(
		config_get_string_value(config, "IP"),
		config_get_string_value(config, "PUERTO")
	);

	op_code cod_op; //Agregado (meter dentro de armado de paquete)
	cod_op=ingresoCodigoOperacion(); //Agregado (Meter dentro del armado de paquete)

	t_paquete* paquete = armar_paquete(cod_op); //Parametro agregado

	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}

t_log* iniciar_logger() {
	return log_create("KernelCliente.log", "KernelCliente", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	return config_create("KernelCliente.config");
}

void leer_consola(t_log* logger) {
	void loggear(char* leido) {
		log_info(logger, leido);
	}

	_leer_consola_haciendo((void*) loggear);
}

//t_paquete* armar_paquete() { //Original
//	t_paquete* paquete = crear_paquete();
//	t_log* logger = iniciar_logger();
//
//	void _agregar(char* leido) {
//
//		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
//		log_info(logger, leido);
//	}
//
//	_leer_consola_haciendo((void*) _agregar);
//
//	return paquete;
//}

t_paquete* armar_paquete(op_code cod_op) { //Agregado
	t_paquete* paquete = crear_paquete(cod_op);
	t_log* logger = iniciar_logger();

	void _agregar(char* leido) {

		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		log_info(logger, leido);
	}

	_leer_consola_haciendo((void*) _agregar);

	return paquete;
}

void _leer_consola_haciendo(void(*accion)(char*)) {
	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0) {
		accion(leido);
		leido = readline(">");
	}

	free(leido);
}

op_code ingresoCodigoOperacion(){ //Agregado

	op_code cod_op;
	int cod_ingresado;

	printf("\n0.PAQUETE\n1.CREATE\n2.DROP\n3.DESCRIBE\n4.SELECT\n5.INSERT\n6.JOURNAL\n7.RUN\n8.ADD\n");

	printf("Ingrese el Codigo de operacion correspondiente\n");
    scanf("%d", &cod_ingresado);

	switch(cod_ingresado){
			case 0:
				cod_op = PAQUETE;
				break;
			case 1:
				cod_op = CREATE;
				break;
			case 2:
				cod_op = DROP;
				break;
			case 3:
				cod_op = DESCRIBE;
				break;
			case 4:
				cod_op = SELECT;
				break;
			case 5:
				cod_op = INSERT;
				break;
			case 6:
				cod_op = JOURNAL;
				break;
			case 7:
				cod_op = RUN;
				break;
			case 8:
				cod_op = ADD;
				break;
			default:
				printf("Operacion desconocida.");
				break;
			}


	return cod_op;
}


