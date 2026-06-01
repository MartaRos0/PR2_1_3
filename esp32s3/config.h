/*
Archivo: config.h
Descripción: Archivo central de configuración del proyecto. Contiene constantes,
credenciales, parámetros globales y ajustes generales del sistema.
*/

// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
#define DEVICE_GIIROB_PR2_ID      "11" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "TestESP32" // Para probar el codigo cambiarte este y el NET_PASSWD (abajo) por la conexion wifi disponible
#define NET_PASSWD                "12345678"

// MQTT
#define MQTT_SERVER_IP            "broker.emqx.io"
#define MQTT_SERVER_PORT          1883
#define MQTT_USERNAME             "PR2_1_3"   
#define MQTT_PASSWORD             "public"

#define ENVIAR_CONJ               "PR2_1_3/linea1/esp"
#define LEER_CONJ                 "PR2_1_3/linea1/python"

// IO
#define LED_PLANTA                4
#define BUTTON_EMERGENCIA         18
#define BUTTON_ACTIVAR_PLANTA     19