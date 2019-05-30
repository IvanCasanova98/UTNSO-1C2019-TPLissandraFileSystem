#include "criterio.h"

SEED elegir_memoria()
{
	SEED memoria;
	int rnd = numero_random(2);

	memoria.PUERTO= seed[rnd].PUERTO;
	memoria.IP = strtok(seed[rnd].IP,"\"");

	printf("\nCRITERIO POR DOS MEMORIA ELEGIDA: IP: %s, PUERTO: %s ",memoria.IP, memoria.PUERTO);
	return memoria;
}
