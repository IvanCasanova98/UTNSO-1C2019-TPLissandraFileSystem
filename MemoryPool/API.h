#ifndef API_H_
#define API_H_

#include "envio.h"
#include "conexion.h"
#include "tablas.h"

void selectf(int cliente_fd, t_paquete_select* paquete, t_config* config, t_log* logger);
void insert(t_paquete_insert* paquete, t_config* config, t_log* logger);
void create(t_paquete_create* paquete, t_config* config, t_log* logger);

#endif /* API_H_ */
