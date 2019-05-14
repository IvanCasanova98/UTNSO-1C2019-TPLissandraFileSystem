#ifndef API_H_
#define API_H_

#include "envio.h"
#include "conexion.h"
#include "paquete.h"


void enviar_select_lissandra(t_paquete_select* paquete);
void selectf(t_paquete_select* paquete);

#endif /* API_H_ */
