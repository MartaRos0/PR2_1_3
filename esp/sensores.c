/*
Archivo: sensores.c
Descripción: Implementación de las funciones de lectura de sensores. Proporciona
acceso a los valores de botones.
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <Arduino.h>
#include "actuadores.h"

/* Private define ------------------------------------------------------------*/
#define SETA_PIN  18
#define BUTTON_PIN 19

/* Exported functions --------------------------------------------------------*/

void seta_init(void) // Funcion que inicializa ambos botones en modo INPUT (entrada)
{
  pinMode(SETA_PIN, INPUT_PULLUP);    // El modo se encuentra en INPUT_PULLUP para evitar estados flotantes y falsos positivos.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool get_seta(void)    // Funcion del boton de emergencia que develve un balor boleano dependiendo de que como se encuentre el estado del boton
{
  int value = digitalRead(SETA_PIN); // Lee el estado del pin
  if(value == 0)      // Si esta pulsado devuelve true
  {
    return true;
  }
  else                // Si no esta pulsado, devuelve false
  {
    return false;
  }
}

bool get_button(void) // Funcion del boton de inicio de la planta que sigue la misma logica que la funcion anterior
{
  int value = digitalRead(BUTTON_PIN);
  if(value == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/

uint8_t template_init_sensores(uint16_t peters)
{
    static int8_t counter;
       
    return peters+1;
}

/* End of file ****************************************************************/
