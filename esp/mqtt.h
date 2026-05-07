/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

/*
Archivo: mqtt.h
Descripcion: Gestión de la conexión MQTT sobre WebSocket (WSS port 8084)
Incluye cola FreeRTOS, serialización/deserialización JSON y reconexión automática.
*/

/* Includes ------------------------------------------------------------------*/
#include <PubSubClient.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "wifi.h"
#include "WsClientWrapper.h"
#include "actuadores.h"

/* Private types -------------------------------------------------------------*/
// Estructura del mensaje para la cola
typedef struct {
  char topic[TOPIC_LEN];
  char serialize_msg[MSG_LEN];
} Mqtt_Message_t;

/* Globals -------------------------------------------------------------------*/
WebSocketsClient webSocket;
WsClientWrapper  wsWrapper(webSocket);
PubSubClient     mqttClient(wsWrapper);

static QueueHandle_t mqtt_queue = NULL;

const char   *mqttServerIP   = MQTT_SERVER_IP;
unsigned int  mqttServerPort = MQTT_SERVER_PORT;

/* Functions -----------------------------------------------------------------*/

void json_message(const char *topic, const char *tipo) // Funcion que crea el mensaje JSON y lo mete a la cola
{
  JsonDocument doc;
  doc["tipo"] = tipo;

  Mqtt_Message_t msg;
  strncpy(msg.topic, topic, sizeof(msg.topic));
  serializeJson(doc, msg.serialize_msg);

  if (xQueueSend(mqtt_queue, &msg, 0) != pdTRUE)  // Si la cola esta llena, descarta el mensaje
    Serial.println("Cola llena, mensaje descartado");
}

void onMessage(char *topic, byte *payload, unsigned int length) // Callback
{
  String payloadStr;
  for (unsigned int i = 0; i < length; i++) 
  {
    payloadStr += (char)payload[i];
  }

  Serial.print("Message arrived in topic: "); 
  Serial.println(topic);
  Serial.print("Message: ");                  
  Serial.println(payloadStr);

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payloadStr);
  if (err)       // Si no se puede deserializar el mensaje lanza un mensaje de error
  { 
    Serial.print("Error deserializando: "); 
    Serial.println(err.c_str()); 
    return; 
  }

  const char *tipo = doc["tipo"];
  if (tipo == nullptr)   // Si no encuentra el campo donde se encuentra el mensaje, lanza un mensaje
  { 
    Serial.println("Campo tipo no encontrado"); 
    return; 
  }
  Serial.println(tipo);

  if (strcmp(tipo, "PLANTA_ACTIVA") == 0)  // Si llega PLANTA_ACTIVA se enciende el led
  { 
    set_led(true);  
    Serial.println("ENCENDIDO"); 
  } 
  else if (strcmp(tipo, "PLANTA_INACTIVA") == 0)   // Si llega PLANTA_INACTIVA se apaga el led
  { 
    set_led(false); 
    Serial.println("APAGADO");   
  }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)  // Callback del websocket
{
  if (type == WStype_CONNECTED) // Si detecta que se conectado lanza mensaja de conexion
  {
    if (!WiFi.isConnected()) return; // Si no se ha conectado al wifi, no continua
    Serial.println("WebSocket conectado");
    delay(1000);

    String client_id = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX); // Genera una ID
    if (mqttClient.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))  // Se intenta conectar al mqtt con la ID y las credenciales
    {
      Serial.println("Broker MQTT conectado");
      mqttClient.subscribe(TOPIC_SUB);          // Se susbcribe al topic
      Serial.println("Subscribed to: " TOPIC_SUB);
      json_message(TOPIC_PUB, "Hi, I'm ESP ^^"); // Publica primer mensaje
    }
    else // Si no se ha conseguido conectar lanza un mensaje con el error generado
    {
      Serial.print("MQTT connect fail, rc=");
      Serial.println(mqttClient.state());
    }
  }
  else if (type == WStype_BIN)
  {
    wsWrapper.injectData(payload, length);
    mqttClient.loop();
  }
  else if (type == WStype_DISCONNECTED)  // Si el websocket se se desconecta lanza mensaje
  {
    Serial.println("WebSocket desconectado - reintentando...");
  }
}

void mqtt_connect() // Funcion que inicializa las conecxiones
{
  mqtt_queue = xQueueCreate(QUEUE_SIZE, sizeof(Mqtt_Message_t)); // Creacion de la cola

  webSocket.beginSSL(mqttServerIP, mqttServerPort, "/mqtt", "", "mqtt"); //Inicia conexion websocket
  webSocket.onEvent(webSocketEvent);   // Asocia la funcion al manejador de eventos del websocket
  webSocket.setReconnectInterval(5000);   // Intervalo de tiempo por el que se intenta conectar cuando se pierde la conexion
  webSocket.enableHeartbeat(25000, 5000, 3);  

  mqttClient.setServer(mqttServerIP, mqttServerPort); // Indica el IP y puerto del mqtt
  mqttClient.setCallback(onMessage);  // Indica que funcion hace de callback
  mqttClient.setKeepAlive(60);  // Hace que mqtt se mantenga abierto
  mqttClient.setSocketTimeout(10);  //Timepo maximo de espera para operaciones de socket

  Serial.println("Esperando conexion WebSocket...");
  unsigned long start = millis();
  while (!mqttClient.connected() && millis() - start < 30000)  // Espera 30s a que el websocket se conecte con el mqtt
  {
    webSocket.loop(); 
    delay(10);
  }

  if (mqttClient.connected()) // Si se conecta lanza un mensaje de existo
    Serial.println("MQTT listo");
  else                        // Sino lanza un mensaje de que lo volvera a intentar
    Serial.println("MQTT no conectado, reintentara en el loop");
}

void mqtt_loop()  // Funcion que conecta lanza los mensajes extrayendolos de la cola
{
  webSocket.loop();

  if (!mqttClient.connected()) return;  // Si no esta conectado el cliente no sigue

  Mqtt_Message_t msg;
  while (xQueueReceive(mqtt_queue, &msg, 0) == pdTRUE)
  {
    mqttClient.publish(msg.topic, msg.serialize_msg);
    Serial.printf("Topic: %s, Msg: %s\n", msg.topic, msg.serialize_msg);
  }
}

#endif
/*** End of file **************************************************************/
