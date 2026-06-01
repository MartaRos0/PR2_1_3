/*
Archivo: actuadores.h
Descripción: Declaración de funciones para controlar actuadores físicos,
como LEDs.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ACTUADORES_H
#define ACTUADORES_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported functions --------------------------------------------------------*/

void led_init(void);
void set_led(bool state);

uint8_t template_init_actuadores(uint16_t peters);


#ifdef __cplusplus
}
#endif


#endif
/*** End of file **************************************************************/
