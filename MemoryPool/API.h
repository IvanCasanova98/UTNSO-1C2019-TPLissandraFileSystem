#ifndef API_H_
#define API_H_

#include "envio.h"
#include "conexion.h"

void selectf(t_paquete_select* paquete, t_config* config, t_log* logger);
void insert(t_paquete_insert* paquete, t_config* config, t_log* logger);

#endif /* API_H_ */
