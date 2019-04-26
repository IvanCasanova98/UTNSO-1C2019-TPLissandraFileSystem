#include "kernel.h"

int main(void){

	t_log* logger = iniciar_logger();

	t_config* config = leer_config();

	int conexion = iniciar_conexion(logger, config);//Iniciar conexion

	armar_paquete(conexion);//Primero pide tipo de paquete, luego lo arma desde "KernelAPI, despues lo envia

	//Eliminar paquete con la funcion (ya creada) eliminar_paquete(paquete)

//	terminar_conexion(logger, config, conexion);//Cerrar conexion

}

//---------------ELIGE TIPO DE PAQUETE Y LO MANDA A PAQUETE PARA ARMARLO

void armar_paquete(int conexion){

	int cod_ingresado;
	t_paqueteSELECT* paqueteSELECT;
	t_paqueteINSERT* paqueteINSERT;

	printf("\n0.CREATE\n1.DROP\n2.DESCRIBE\n3.SELECT\n4.INSERT\n5.JOURNAL\n6.RUN\n7.ADD\n");
	printf("\nIngrese el Codigo de operacion correspondiente");
	scanf("\n%d", &cod_ingresado);

	switch(cod_ingresado){
			case 3:
				paqueteSELECT = selectf(); //EN paquete.c LO ARMA
				enviar_paqueteSELECT(paqueteSELECT, conexion); //EN encio.c LO ENVIA
				break;
			case 4:
				paqueteINSERT = insert();
				enviar_paqueteINSERT(paqueteINSERT, conexion);
				break;
			default:
				printf("Operacion desconocida.");
				break;
	}
}




