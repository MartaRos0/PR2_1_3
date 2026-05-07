/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CONFIG_H
#define CONFIG_H

/*
Archivo: config.h
Descripción: Archivo central de configuración del proyecto. Contiene constantes,
credenciales, parámetros globales y ajustes generales del sistema.
*/

/* Comm -----------------------------------------------------------------------*/
#define BAUDS 115200

/* Cola MQTT -----------------------------------------------------------------*/
#define QUEUE_SIZE  10
#define MSG_LEN     150
#define TOPIC_LEN   100

/* Device --------------------------------------------------------------------*/
#define DEVICE_ID   "esp32-client"

/* WiFi ----------------------------------------------------------------------*/
#define NET_SSID    "UPV-PSK"
#define NET_PASSWD  "giirob-pr2-2023"

/* MQTT ----------------------------------------------------------------------*/
#define MQTT_SERVER_IP    "broker.emqx.io"
#define MQTT_SERVER_PORT  8084
#define MQTT_USERNAME     "PR2_1_3"
#define MQTT_PASSWORD     "public"

/* Topics --------------------------------------------------------------------*/
#define TOPIC_PUB   "PR2_1_3/linea1/esp"
#define TOPIC_SUB   "PR2_1_3/linea1/python"

/* IO ------------------------------------------------------------------------*/
#define LED_PIN     4
#define SETA_PIN    18
#define BUTTON_PIN  19

#endif
/*** End of file **************************************************************/
