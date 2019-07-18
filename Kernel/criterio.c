#include "criterio.h"

t_list* lista_seeds;
int referencia_memoria_ec; //PUNTERO A LA ULTIMA MEMORIA UTILIZADA POR EVENTUAL CONSISTENCY


SEED * elegir_memoria(char * consistencia_tabla)
{
	t_list * pool_especifico = get_pool(consistencia_tabla); //Tengo todas LOS NUMEROS DE MEMORIA asociados a una consistencia

	SEED * seed_i;
	int numero_memoria;

	int cons_ingresada = consistencia_ingresada(consistencia_tabla);
	switch(cons_ingresada)
	{
	case SC:;
		numero_memoria = list_get(pool_especifico,0);
		break;
	case SHC:;
		//numero_memoria = hash(pool_especifico);
		break;
	case EC:
		numero_memoria = get_memoria_fifo(pool_especifico);
		break;
	default:
		break;
	}

	seed_i = get_seed_especifica(numero_memoria); //PROBLEMA DE ELECCION DE MEMORIA

	return seed_i;
}



//----------------------------------------------UTILS------------------------------
SEED * get_seed_especifica(int numero_memoria)
{
	bool _seed_buscada(void * elemento) {return seed_buscada(elemento,numero_memoria);}

//	printf("BUSCANDO SEED");

	return list_find(lista_seeds, _seed_buscada);
}

bool seed_buscada(SEED * memoria_i , int numero_memoria)
{
	return memoria_i->NUMBER == numero_memoria;
}

int get_memoria_fifo(t_list* pool)
{
	int tope_lista = list_size(pool) - 1;
	int numero_memoria = 0;

	if (referencia_memoria_ec == tope_lista)
	{
		referencia_memoria_ec = 0;
	}
	else
	{
		referencia_memoria_ec ++;
	}

	numero_memoria = list_get(pool, referencia_memoria_ec);

	return numero_memoria;
}
//----------------------------------------------------------------------------------------

void startup_lista_seeds()
{
	referencia_memoria_ec = -1;
	lista_seeds = list_create();
}

void mostrar_lista_seeds()
{
	list_iterate(lista_seeds,imprimir);
}

void imprimir(SEED * seed){
	printf("\nMemoria: %d - Puerto: %d - Ip: %s", seed->NUMBER,seed->PUERTO,seed->IP);
}

