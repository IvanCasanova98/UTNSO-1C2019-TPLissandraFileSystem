#include "utils.h"

int numero_random(int max)
{
	srand (getpid());
	int rnd = rand() % max;
	return rnd;
}
