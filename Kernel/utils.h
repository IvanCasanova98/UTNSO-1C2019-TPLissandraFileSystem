#ifndef UTILS_H_
#define UTILS_H_

typedef struct SEED
{
	char* IP;
	char* PUERTO;
}SEED;

SEED seed[];

int numero_random(int max);

#endif /* UTILS_H_ */
