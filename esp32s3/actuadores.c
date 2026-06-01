/*
Archivo: actuadores.c
Descripción: Implementación de las funciones de control de actuadores. Gestiona
el encendido, apagado e inicializacion salidas digitales.
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <Arduino.h>
#include "config.h"
#include "actuadores.h"

/* Private define ------------------------------------------------------------*/
#define LED_PIN   LED_PLANTA

/* Exported functions --------------------------------------------------------*/

void led_init(void)
{
  pinMode(LED_PIN, OUTPUT);
}

void set_led(bool state)
{
  if (state)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/

uint8_t template_init_actuadores(uint16_t peters)
{
    static int8_t counter;
       
    return peters+1;
}

/* End of file ****************************************************************/
