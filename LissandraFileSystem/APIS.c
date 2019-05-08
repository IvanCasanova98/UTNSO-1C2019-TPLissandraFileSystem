

#include "APIS.h"

int existeTabla(char* TablaBuscada){

	char *directorioTablas = malloc( sizeof(DIRECTORIOTABLAS) + sizeof(TablaBuscada) );
	strcpy(directorioTablas,DIRECTORIOTABLAS);
	strcat(directorioTablas,TablaBuscada);
	//directorioTablas termina siendo el directorio en especifico buscado.
	//por ejemplo "/home/utnso/lfs/LissandraFileSystem/Tablas/TablasA"

	DIR* dir = opendir(directorioTablas);
	if (dir)
	{
	    closedir(dir);
	    free(directorioTablas);
	    return 1;
	}
	else if (ENOENT == errno)
	{
	    free(directorioTablas);
	    return 0;
	}
	else
	{
		/* Fallo por otra razon */
		printf("Fallo desconocido :(\n");
		free(directorioTablas);
	    return 0;
	}
}
