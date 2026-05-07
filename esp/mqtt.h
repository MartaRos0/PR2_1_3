/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

/*
Archivo: mqtt.h
Descripcion: Gestión de la conexión MQTT sobre WebSocket (WSS port 8084)
Incluye cola FreeRTOS para publicación thread-safe,
serialización/deserialización JSON y reconexión automática.
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

void json_message(const char *topic, const char *tipo)
{
  JsonDocument doc;
  doc["tipo"] = tipo;

  Mqtt_Message_t msg;
  strncpy(msg.topic, topic, sizeof(msg.topic));
  serializeJson(doc, msg.serialize_msg);

  if (xQueueSend(mqtt_queue, &msg, 0) != pdTRUE)
    Serial.println("Cola llena, mensaje descartado");
}

void onMessage(char *topic, byte *payload, unsigned int length)
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
  if (err) 
  { 
    Serial.print("Error deserializando: "); 
    Serial.println(err.c_str()); 
    return; 
  }

  const char *tipo = doc["tipo"];
  if (tipo == nullptr) 
  { 
    Serial.println("Campo tipo no encontrado"); 
    return; 
  }
  Serial.println(tipo);

  if (strcmp(tipo, "PLANTA_ACTIVA") == 0) 
  { 
    set_led(true);  
    Serial.println("ENCENDIDO"); 
  } 
  else if (strcmp(tipo, "PLANTA_INACTIVA") == 0) 
  { 
    set_led(false); 
    Serial.println("APAGADO");   
  }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_CONNECTED)
  {
    if (!WiFi.isConnected()) return;
    Serial.println("WebSocket conectado");
    delay(1000);

    String client_id = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqttClient.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("Broker MQTT conectado");
      mqttClient.subscribe(TOPIC_SUB);
      Serial.println("Subscribed to: " TOPIC_SUB);
      json_message(TOPIC_PUB, "Hi, I'm ESP ^^");
    }
    else
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
  else if (type == WStype_DISCONNECTED)
  {
    Serial.println("WebSocket desconectado - reintentando...");
  }
}

void mqtt_connect()
{
  mqtt_queue = xQueueCreate(QUEUE_SIZE, sizeof(Mqtt_Message_t));

  webSocket.beginSSL(mqttServerIP, mqttServerPort, "/mqtt", "", "mqtt");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(25000, 5000, 3);

  mqttClient.setServer(mqttServerIP, mqttServerPort);
  mqttClient.setCallback(onMessage);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(10);

  Serial.println("Esperando conexion WebSocket...");
  unsigned long start = millis();
  while (!mqttClient.connected() && millis() - start < 30000)
  {
    webSocket.loop(); 
    delay(10);
  }

  if (mqttClient.connected())
    Serial.println("MQTT listo");
  else
    Serial.println("MQTT no conectado, reintentara en el loop");
}

void mqtt_loop()
{
  webSocket.loop();

  if (!mqttClient.connected()) return;

  Mqtt_Message_t msg;
  while (xQueueReceive(mqtt_queue, &msg, 0) == pdTRUE)
  {
    mqttClient.publish(msg.topic, msg.serialize_msg);
    Serial.printf("Topic: %s, Msg: %s\n", msg.topic, msg.serialize_msg);
  }
}

#endif
/*** End of file **************************************************************/
