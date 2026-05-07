/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SENSORES_H
#define SENSORES_H

/*
Archivo: sensores.h
Descripción: Declaración de funciones para leer sensores físicos del sistema,
como botones.
*/

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported functions --------------------------------------------------------*/

void seta_init(void);
bool get_seta(void);
bool get_button(void);

uint8_t template_init_sensores(uint16_t peters);


#ifdef __cplusplus
}
#endif


#endif
/*** End of file **************************************************************/
