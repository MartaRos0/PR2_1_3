/*
Archivo: main.ino
Descripcion: Programa principal. Inicializa el sistema, crea las tareas FreeRTOS
para la lectura de botones y ejecuta el loop principal de MQTT.
*/


/* Includes ------------------------------------------------------------------*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "wifi.h"
#include "mqtt.h"
#include "WsClientWrapper.h"
#include "actuadores.h"
#include "sensores.h"

/* Private variables ---------------------------------------------------------*/
static TaskHandle_t emergency_stop_button_handle = NULL;
static TaskHandle_t start_button_handle = NULL;

/* Private function ----------------------------------------------------------*/
void emergency_stop_button(void *parameter);
void start_button(void *parameter);

void setup() // Inicializa pines, gestiona conexiones y crea tareas
{
  Serial.begin(BAUDS);
  led_init();
  seta_init();

  wifi_connect();
  mqtt_connect();

  xTaskCreatePinnedToCore(emergency_stop_button, "seta",  10000, NULL, 1, &emergency_stop_button_handle, 0);
  xTaskCreatePinnedToCore(start_button, "boton", 10000, NULL, 1, &start_button_handle, 0);
  Serial.println("Tareas creadas");
}

void emergency_stop_button(void *parameter) // Detecta pulsos del boton de emergencia
{
  bool state_before = false; 
  for (;;)
  {
    bool state = get_seta();
    if (state && !state_before) 
    { 
      json_message(TOPIC_PUB, "EMERGENCIA"); 
      Serial.println("EMERGENCIA encolada"); 
    }
    state_before = state;
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void start_button(void *parameter)  // Detecta pulsos del boton de incio 
{
  bool state_before = false;
  for (;;)
  {
    bool state = get_button();
    if (state && !state_before) 
    { 
      json_message(TOPIC_PUB, "EMPIEZA"); 
      Serial.println("EMPIEZA encolada"); 
    }
    state_before = state;
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void loop() // Mantiene conexiones
{
  wifi_loop();
  mqtt_loop();
}
