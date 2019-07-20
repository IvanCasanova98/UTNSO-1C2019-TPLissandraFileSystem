#include "recibir.h"


#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

#ifndef KERNEL_PROTOCOLO_H_
#define KERNEL_PROTOCOLO_H_


void protocolo_respuesta(uint16_t codigo_respuesta,t_log* logger);

#endif /* KERNEL_PROTOCOLO_H_ */
