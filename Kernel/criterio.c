#include "criterio.h"

t_list* lista_seeds;

//SEED elegir_memoria()
//{
//	SEED memoria;
//	int rnd = numero_random(2);
//
//	memoria.PUERTO= seed[rnd].PUERTO;
//	memoria.IP = strtok(seed[rnd].IP,"\"");
//
//	printf("\nCRITERIO POR DOS MEMORIA ELEGIDA: IP: %s, PUERTO: %s ",memoria.IP, memoria.PUERTO);
//	return memoria;
//}


SEED * elegir_memoria(char * consistencia_tabla){
//para mi no llega el nombre tabla, hay q ver por request, pero bueno, es para una primera
//aproximacion. Fijense dsp

	t_list * pool_especifico = get_pool(consistencia_tabla); //Tengo todas LOS NUMEROS DE MEMORIA asociados a una consistencia

	SEED * seed_i;



	int cons_ingresada = consistencia_ingresada(consistencia_tabla);
	switch(cons_ingresada)
	{
	case SC:;
		//Hay una sola memoria en este caso, asi que elijo la unica
		//Como hay una sola, tomo el numero de memoria del pool (es lo que creamos en memorias.c como agregar consistencia
		// y dsp agarro la seed especifica de ese numero de memoria (q la conozco por la lista de seed que recibo en
		// el inicio del kernel. esta en este header) y la devuelvo

		int numero_memoria;
		numero_memoria = list_get(pool_especifico,0);
		seed_i = get_seed_especifica(numero_memoria);

		break;
	case SHC:;
		break;
	case EC:
		break;
	default:
		break;
	}


	return seed_i;
}



//----------------------------------------------UTILS------------------------------
SEED * get_seed_especifica(int numero_memoria)
{

	bool _seed_buscada(void * elemento) {return seed_buscada(elemento,numero_memoria);}

	return list_find(lista_seeds, _seed_buscada);
}

bool seed_buscada(SEED * memoria_i , int numero_memoria)
{
	return memoria_i->NUMBER == numero_memoria;
}
//----------------------------------------------------------------------------------------

void startup_lista_seeds()
{
	lista_seeds = list_create();
}

void mostrar_lista_seeds()
{
	list_iterate(lista_seeds,imprimir);
}

void imprimir(SEED * seed){
	printf("\nMemoria: %d - Puerto: %d - Ip: %s", seed->NUMBER,seed->PUERTO,seed->IP);
}

