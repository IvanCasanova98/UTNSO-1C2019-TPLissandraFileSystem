#ifndef PAQUETE_H_
#define PAQUETE_H_

typedef enum
{
	CREATE,
	DROP,
	DESCRIBE,
	SELECT,
	INSERT,
	JOURNAL,
	RUN,
	ADD
}op_code;

typedef struct t_paquete_select
{
	char* nombre_tabla;
	uint32_t nombre_tabla_long;
	uint16_t valor_key;
}t_paquete_select;

typedef struct t_paquete_insert
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	uint32_t value_long; 		//Longitud del value
	char* nombre_tabla;
	uint16_t valor_key;
	char* value;
	int timestamp;
}t_paquete_insert;

#endif /* PAQUETE_H_ */
