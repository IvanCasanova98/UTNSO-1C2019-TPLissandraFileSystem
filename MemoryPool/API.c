#include "API.h"

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
