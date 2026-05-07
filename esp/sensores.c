/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <Arduino.h>
#include "actuadores.h"

/* Private define ------------------------------------------------------------*/
#define SETA_PIN  18
#define BUTTON_PIN 19

/* Exported functions --------------------------------------------------------*/

void seta_init(void)
{
  pinMode(SETA_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool get_seta(void)
{
  int value = digitalRead(SETA_PIN);
  if(value == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool get_button(void)
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
