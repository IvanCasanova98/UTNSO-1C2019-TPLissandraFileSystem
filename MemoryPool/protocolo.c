#include "protocolo.h"


void protocolo_respuesta(uint16_t codigo_respuesta,t_log* logger)
{
		switch(codigo_respuesta)
		{
		//********************************** CREATE(0)
			case 90:;
			log_info(logger,"Se creó la tabla en el filesystem con éxito.");
				break;
			case 91:;
			log_warning(logger,"ERROR ingreso request CREATE.");
				break;
			case 92:;
			log_warning(logger,"La tabla ya existe en el filesystem.");
				break;
			case 93:;
			log_warning(logger,"ERROR recibo de paquete CREATE.");
				break;
		//********************************** DROP(1)
			case 10:;
			log_info(logger,"Se eliminó la tabla del filesystem con éxito.");
				break;
			case 11:;
			log_warning(logger,"ERROR ingreso request DROP.");
				break;
			case 12:;
			log_warning(logger,"La tabla NO existe en el filesystem.");
				break;
			case 13:;
			log_warning(logger,"ERROR recibo de paquete DROP.");
				break;
		//********************************** DESCRIBE(2)
			case 20:;
			log_info(logger,"DESCRIBE de tabla:");
				break;
			case 21:;
			log_warning(logger,"ERROR ingreso request DESCRIBE.");
				break;
			case 22:;
			log_warning(logger,"NO existe la tabla a describir solicitada.");
				break;
			case 23:;
			log_warning(logger,"NO hay ninguna tabla en el filesystem para describir.");
				break;
			case 24:;
			log_warning(logger,"ERROR recibo de paquete DESCRIBE.");
				break;
			case 25:;
				log_info(logger,"DESCRIBE de todas las tablas:");
			break;
		//********************************** SELECT(3)
			case 31:;
			log_warning(logger,"ERROR ingreso request SELECT.");
				break;
			case 32:;
			log_warning(logger,"NO se encuentra la key solicitada.");
				break;
			case 33:;
			log_warning(logger,"La tabla NO existe en el filesystem.");
				break;
			case 34:;
			log_warning(logger,"ERROR recibo de paquete SELECT.");
				break;
		//********************************** INSERT(4)
			case 40:;
			log_info(logger,"Se insertó la key en la memtable con éxito.");
				break;
			case 41:;
			log_warning(logger,"ERROR ingreso request INSERT.");
				break;
			case 42:;
			log_warning(logger,"NO existe la tabla a insertar.");
				break;
			case 43:;
			log_warning(logger,"ERROR recibo de paquete INSERT.");
				break;

			default:
				printf("\n Unknown cod rta: %d\n",codigo_respuesta);
				break;
		}

}
