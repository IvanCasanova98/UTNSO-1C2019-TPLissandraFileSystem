//Hace falta un log destroy por cada creacion del logger??

#include "kernel.h"

int main(void){
	t_log* logger = iniciar_logger();

	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);//Iniciar conexion


	armar_paquete(conexion);//Primero pide tipo de paquete, luego lo arma desde "KernelAPI, despues lo envia

	terminar_conexion(logger,config,conexion);


}

//---------------ELIGE TIPO DE PAQUETE Y LO MANDA A PAQUETE PARA ARMARLO

void armar_paquete(int conexion){

	int cod_ingresado;
//	t_paquete_select* paquete_select;
//	t_paquete_insert* paquete_insert;
	system("clear");
	printf("\n0.CREATE\n1.DROP\n2.DESCRIBE\n3.SELECT\n4.INSERT\n5.JOURNAL\n6.RUN\n7.ADD\n");
	printf("\nIngrese el Codigo de operacion correspondiente: ");
	scanf("\n%d", &cod_ingresado);

	//Decidi que enviaremos suelto el cod_op.

	while(cod_ingresado!=8){
		switch(cod_ingresado){
					case 3:;
						t_paquete_select* paquete_select = selectf();
						if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
								else {
									t_log* logger = iniciar_logger();
									log_info(logger, "Se envio codigo de operacion: %d",cod_ingresado);
									log_destroy(logger);

								}

						enviar_paquete_select(paquete_select, conexion); //EN envio.c LO ENVIA
						free(paquete_select); //Va este free???s si PAPANATAS
						break;
					case 4:;
						t_paquete_insert* paquete_insert = insert();
						if (send(conexion, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
								else {
									t_log* logger = iniciar_logger();
									log_info(logger, "Se envio codigo de operacion: %d",cod_ingresado);
									log_destroy(logger);

								}
						enviar_paquete_insert(paquete_insert, conexion);
						free(paquete_insert);
						break;
					default:
						printf("Operacion desconocida.");
						break;
			}

//		system("clear");
		printf("\n0.CREATE\n1.DROP\n2.DESCRIBE\n3.SELECT\n4.INSERT\n5.JOURNAL\n6.RUN\n7.ADD\n");
		printf("\nIngrese el Codigo de operacion correspondiente: ");
		scanf("\n%d", &cod_ingresado);

	}
	system("clear");
	t_log* logger = iniciar_logger();
	log_info(logger, "Kernel Desconectado");
	log_destroy(logger);

  }




