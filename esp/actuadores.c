/*
Archivo: actuadores.c
Descripción: Implementación de las funciones de control de actuadores. Gestiona
el encendido, apagado e inicializacion salidas digitales.
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <Arduino.h>
#include "actuadores.h"

/* Private define ------------------------------------------------------------*/
#define LED_PIN   4

/* Exported functions --------------------------------------------------------*/

void led_init(void)      // Funcion para inicialiar el led en modo OUTPUT (salida)
{
  pinMode(LED_PIN, OUTPUT);
}

void set_led(bool state)      // Funcion que dependiendo del parametro que se le añada (true o false) se enciende o apaga el led.
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
