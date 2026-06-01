/*
Archivo: main.ino
Descripcion: Programa principal. Inicializa el sistema, crea las tareas FreeRTOS
para la lectura de botones, incluye cola FreeRTOS, serialización JSON y ejecuta el loop principal.
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "actuadores.h"
#include "sensores.h"

// Definicion de pines y delays de tareas
#define EMERGENCY_BUTTON          BUTTON_EMERGENCIA
#define TEMP_PIN                  TEMPERATURE_SENSOR
#define TIME_DELAY_TEMP_ESP       10000
#define TIME_DELAY_START_BUTTON   50

// MQTT Broker
String deviceID = String("giirobpr2-device-") + String(DEVICE_GIIROB_PR2_ID); 

// Variables Cola
#define SIZE 10
#define MSG_LEN 150

// Estructura del mensaje
typedef struct {
  char topic[100];
  char serialize_msg[MSG_LEN];
} Mqtt_Message_t;

// Estructura para almacenar los paremetros que se pasaran a las tareas
typedef struct {
  int periodo_delay_temp_esp;
  int periodo_delay_start_button;
  QueueHandle_t queue_button;
} TempParams_t;

// Inicializacion de la cola, parametros y varible de interruccion
static QueueHandle_t mqtt_queue = NULL;
TempParams_t *parameters = NULL;
volatile bool emergency = false;

// Variables Tareas
static TaskHandle_t start_button_handle = NULL;
static TaskHandle_t temperature_sensor_handle = NULL; 

void start_button(void * parameter);
void temperature_esp ( void * parameter);

// Funcion de la interrupcion
void IRAM_ATTR emergency_ISR() 
{
  emergency = true;
}

// Funcion que crea los mensajes JSON y los mete a la cosa o descarta si esta esta llena
void json_message(QueueHandle_t queue, const char *topic, const char *tipo)
{
  JsonDocument doc;
  doc["tipo"] = tipo;
  Mqtt_Message_t msg;
  strncpy(msg.topic, topic, sizeof(msg.topic));
  serializeJson(doc, msg.serialize_msg);

  if (xQueueSend(queue, &msg, 0) != pdTRUE)
  {
    Serial.println("Cola llena, mensaje descartado");
  }
}

void setup() 
{
  Serial.begin(115200);
  led_init();
  sensor_init();

  //Cola
  mqtt_queue = xQueueCreate(SIZE, sizeof(Mqtt_Message_t));

  //WiFi
  wifi_connect();

  // MQTT
  mqtt_connect(deviceID);
  mqtt_subscribe(LEER_CONJ);

  //Interruccion
  attachInterrupt(EMERGENCY_BUTTON, emergency_ISR, FALLING);

  //Parametros de las tareas
  parameters = new TempParams_t;
  parameters->periodo_delay_temp_esp = TIME_DELAY_TEMP_ESP;
  parameters->periodo_delay_start_button = TIME_DELAY_START_BUTTON;
  parameters->queue_button = xQueueCreate(SIZE, sizeof(Mqtt_Message_t));

  //Tareas
  xTaskCreatePinnedToCore(start_button, "start_button", 10000, parameters, 1, &start_button_handle, 0);
  xTaskCreatePinnedToCore(temperature_esp, "temperature_esp", 10000, parameters, 1, &temperature_sensor_handle, 0);
  Serial.print("Tareas Creadas\n\r");
}

// Tarea que indica que se ha pulsado el boton de iniciar
void start_button ( void * parameter)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  TempParams_t *p = (TempParams_t*) parameter;
  bool state_before = false;
  for(;;)
  {
    bool state = get_button();
    if(state == true && state_before == false)
    {
      json_message(p->queue_button, ENVIAR_CONJ, "EMPIEZA");
      Serial.print("EMPIEZA encolada");
    }
    state_before = state;
    vTaskDelayUntil(&xLastWakeTime,  pdMS_TO_TICKS(p->periodo_delay_start_button));
  }
}

// Tarea que realiza una comprobacion periodica de la temperatura de la esp
void temperature_esp ( void * parameter)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  TempParams_t *p = (TempParams_t*) parameter;
  for(;;)
  {
    float value = temperatureRead();
    if(value >= 65.0)
    {
      emergency = true;
    }
    else
    {
      Serial.printf("TEMPERATURA ESP: %.2f\n", value);
    }
    vTaskDelayUntil(&xLastWakeTime, p->periodo_delay_temp_esp / portTICK_PERIOD_MS);
  }
}

void loop() 
{
  wifi_loop();
  mqtt_loop();

  // Si nos encontramos en estado de emergencia lanza directamente el mensaje sin pasar por colas que ralenticen el envio
  if (emergency) 
  {
    JsonDocument doc;
    doc["tipo"] = "EMERGENCIA";
    Mqtt_Message_t msg;
    strncpy(msg.topic, ENVIAR_CONJ, sizeof(msg.topic));
    serializeJson(doc, msg.serialize_msg);
    mqtt_publish(msg.topic,msg.serialize_msg);
    emergency = false; // Una vez lanzado el mensaje se quita el estado de emergencia
  }
  else // Si no nos encontramos en estado de emergencia se realizan envios de mensajes de manera normal, pasando por la cola
  {
    Mqtt_Message_t msg;
    while (xQueueReceive(parameters->queue_button, &msg, 0) == pdTRUE)
    {
      mqtt_publish(msg.topic,msg.serialize_msg);
      Serial.printf("Topic: %s, Msg: %s \n\r", msg.topic, msg.serialize_msg);
    }
  }
}
