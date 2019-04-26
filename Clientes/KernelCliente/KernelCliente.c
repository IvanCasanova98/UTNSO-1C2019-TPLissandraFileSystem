#include "KernelCliente.h"


int main(void){

//	t_log* logger = iniciar_logger();
//	log_info(logger, "CONECTANDO CON MEMORY POOL");
//
//	t_config* config = leer_config();
//
//	int conexion = crear_conexion(
//		config_get_string_value(config, "IP"),
//		config_get_string_value(config, "PUERTO")
//	);

	t_paquete* paquete = construir_paquete(); //Parametro agregado

	//mostrarPaquete(paquete);


//	enviar_paquete(paquete, conexion);
//
//	eliminar_paquete(paquete);
//	log_destroy(logger);
//	config_destroy(config);
//	close(conexion);
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

void _leer_consola_haciendo(void(*accion)(char*)) {
	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0) {
		accion(leido);
		leido = readline(">");
	}

	free(leido);
}

t_paquete* construir_paquete(){ //Agregado

	char valor_key[20];
	char nombre_tabla[20];
	char *tipo_paquete[20]={"CREATE","DROP","DESCRIBE","SELECT","INSERT","JOURNAL","RUN","ADD","PAQUETE"}; //Solo para loggear
	op_code cod_op;
	int cod_ingresado = 0;

	t_paquete* paquete;

	printf("\n0.CREATE\n1.DROP\n2.DESCRIBE\n3.SELECT\n4.INSERT\n5.JOURNAL\n6.RUN\n7.ADD\n8.PAQUETE");
	printf("\nIngrese el Codigo de operacion correspondiente");
    scanf("\n%d", &cod_ingresado);

	switch(cod_ingresado){
			case 0:
				cod_op = CREATE;
				break;
			case 1:
				cod_op = DROP;
				break;
			case 2:
				cod_op = DESCRIBE;
				break;
			case 3:
				cod_op = SELECT;
				printf("\nIngrese nombre de la tabla: ");
				scanf("%s", &nombre_tabla);
				printf("\nIngrese el valor de la key: ");
				scanf("%s", &valor_key);
				paquete = crear_paquete(cod_op,nombre_tabla,valor_key);
				loggear_paquete(paquete, tipo_paquete[3]);

				break;
			case 4:
				cod_op = INSERT;
				break;
			case 5:
				cod_op = JOURNAL;
				break;
			case 6:
				cod_op = RUN;
				break;
			case 7:
				cod_op = ADD;
				break;
			case 8:
				cod_op = PAQUETE;
				break;
			default:
				printf("Operacion desconocida.");
				break;
			}
	return paquete;

}


void mostrarPaquete(t_paquete* paquete){
	printf("%s", paquete->nombre_tabla);
	printf("%s", paquete->valor_key);
}

void loggear_paquete(t_paquete* paquete, char tipo_paquete[10]){

	t_log* logger = iniciar_logger();
	log_info(logger, "Nuevo paquete creado");
	log_info(logger, tipo_paquete);
	log_info(logger, paquete->nombre_tabla);
	log_info(logger, paquete->valor_key);
}
