#include "KernelCliente.h"


int main(void){

	t_log* logger = iniciar_logger();
	log_info(logger, "CONECTANDO CON MEMORY POOL");

	t_config* config = leer_config();

	int conexion = crear_conexion(
		config_get_string_value(config, "IP"),
		config_get_string_value(config, "PUERTO")
	);

	t_paquete* paquete = construirDatos(); //Parametro agregado

	mostrarPaquete(paquete);

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

//t_paquete* armar_paquete(op_code cod_op, char , int key) { //Agregado
//
//	t_paquete* paquete = crear_paquete(cod_op, nombreTabla, key);
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

t_paquete* construirDatos(){ //Agregado
	op_code cod_op;
	int cod_ingresado = 0;

	int valorKey=10;
	char nombreTabla[20]={'t','a','b','l','a'};
	t_paquete* paquete;
	printf("\n0.CREATE\n1.DROP\n2.DESCRIBE\n3.SELECT\n4.INSERT\n5.JOURNAL\n6.RUN\n7.ADD\n");

	printf("\nIngrese el Codigo de operacion correspondiente\n");
    scanf("%d", &cod_ingresado);
    printf("asdasd");
	switch(cod_ingresado){
			case 0:
				cod_op = CREATE; //tipo paquete 2
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			case 1:
				cod_op = DROP; //tipo paquete 1
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			case 2:
				cod_op = DESCRIBE; //tipo paquete 1
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			case 3:

				cod_op = SELECT; //tipo paquete 1

				printf("asdasd");

				paquete = crear_paquete(cod_op,nombreTabla, valorKey);

				break;
			case 4:
				cod_op = INSERT; //tipo paquete 1
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			case 5:
				cod_op = JOURNAL;  //tipo paquete 3 (comando)
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			case 6:
				cod_op = RUN;
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			case 7:
				cod_op = ADD;
				//armar_paquete(cod_op, nombre_tabla, Key, value, timestamp, tipo consistencia, numero particiones, compaction time, FLAG journal, memoria,  numero)
				break;
			default:
				printf("Operacion desconocida.");
				break;
			}



//	t_log* logger = iniciar_logger();
//	t_paquete* paquete = crear_paquete(cod_op,nombreTabla, valorKey);
//
//	void _agregar(char* leido) {
//
//		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
//		log_info(logger, leido);
//	}
//
//	_leer_consola_haciendo((void*) _agregar);
//

	return paquete;

}

void mostrarPaquete(t_paquete* paquete){
	printf("%s", paquete->nombreTabla);
	printf("%d", paquete->key);
}

