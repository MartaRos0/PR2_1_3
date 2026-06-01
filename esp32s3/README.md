# README-ESP32S3
## Descripción general

Este modulo contiene el codigo desarrollado para la ESP32-S3, el cual es el encagado de:
* Leer botones fisicos que actuan como sensores
* Controlar leds que actuan como actuadores
* Enviar y recibir mensajes JSON mediante MQTT
* Ejecutar tareas concurrentes
* Integrarse con el resto del proyecto

## Estructura del proyecto
```
esp32/
│
├── main.ino          → Lógica principal, MQTT, FreeRTOS, colas
├── actuadores.c      → Control de actuadores (LED)
├── actuadores.h
├── sensores.c        → Lectura de sensores (botones)
├── sensores.h
├── mqtt.ino          → Gestión de MQTT
├── b_wifi.ino        → Gestión de WiFi
├── a_logger.ino      → Gestión de mensajes
├── config.h          → Variables Globales
└── README            → Este documento
```
## Tecnologías utilizadas

* ESP32-S3 como microcontrolador principal
* Arduino como entorno de desarrollo
* FreeRTOS para tareas concurrentes
* MQTT y ArduinoJson para comunicacion entre sistemas

## Funcionamiento general

1. La ESP se conecta a la red WiFi usando las credenciales propias (Conexion WiFi)
2. Se conecta al broker MQTT, en este caso:
   * Broker: `broker.emqx.io`
   * Usuario: `PR2_1_3`
   * Puerto: 1883
   * Topic de publicacion `PR2_1_3/linea1/esp`
   * Topic de subscripcion `PR2_1_3/linea1/python`
3. Se crean las tareas FreeRTOS
   * `temperature_esp` que realiza una vigilancia periodica de la temperatura interna del mircroprocesador
   * `start_button` que detecta cuando se pulsa el boton de inicio de funcionamiento de la planta
4. Se crea una interrupcion para el boton de emergencia
   * `emergency` actua como flag
   * `emergency_ISR` activa el flag
5. Se envian mensajes JSON
   * `{"tipo": "EMERGENCIA"}` si se ha pulsado el boton de emergencia
   * `{"tipo": "EMPIEZA"}` si se ha pulsado el boton de inicio
6. Se reciben mensajes JSON
   * `{"tipo": "PLANTA_ACTIVA"}` si la planta ha comenzado su funcionamiento
   * `{"tipo": "PLANTA_INACTIVA"}` si la planta se ha detenido o ha finalizado su funcionamiento
   
## Explicacion de cada archivo

### main.ino
Este es el archivo principal y contiene el `setup()` que gestiona la conexión wifi y mqtt, así como inicializa los sensores, actuadores, parametros y tareas. 
También encontramos el `loop()` que mantiene la conexión wifi y mqtt asi como realiza el envio de los mensajes comprobando constantemente la cola implementada para guardar los eventos lanzados por la tarea y comprobar si la planta se encuetra en estado de emergencia o no. En este archivo también se crean las tareas `temperature_esp()`, tarea que se encarga de mirar el estado de la temperatura interna de la esp, y `star_button()`, tarea que se encarga de mirar el estado del botón que indica si la plante se ha iniciado o no. Por otro lado tambien encontramos la interrupcion `emergency_ISR()` asociada al boton de emergencia y la funcion `json_message()`, encargada de crear los mensajes JSON.

### actuadoes.c / actuadores.h
En estos archivos se encuentran la declaración (`actuadores.h`) e implementación (`actuadores.c`) de las funciones que se encargan de inicializar (`led_init()`), e indicar el estado en el que se deben de encontrar (`set_led()`) de los actuadores (leds que indican si la planta está activa o inactiva dependiendo del mensaje recibido por el topic suscrito).

### sensores.c / sensores.h
En estos archivos se encuentran la declaración (`sensores.h`) e implementación (`sensores.c`) de las funciones que se encargan de inicializar (`sensor_init()`, esta función se encarga de inicializar ambos botones a la vez en modo pull up -leen en Low cuando están pulsados, para no tener estados dudosos-), e indicar en qué estado se han encontrado (`get_seta()`, para el botón de emergencia y `get_button()`, para el botón de inicio) de los sensores (botones que indican si existe alguna emergencia o si la planta debe iniciarse).

### config.h
En este archivo se encuentran todas las constantes de configuracion del sistema. Incluye los parametros de conexion WiFi, 
broker MQTT, topics de publicacion y subscripcion, pines hardware, y parametros de la cola. Se a centralizado para que sea
mas facil su modificacion.

### b_wifi.ino
En este archivo se encuentran las funciones encargadas de gestionar la conexion WiFi. La funcion `wifi_connect()`inicializa
el modulo WiFi y establece la conexion con la red, mientras que `wifi_reconnect()` se encarga de reintentar la conexion en caso de
perdida. La funcion `wifi_loop()` se llama periodicamente para comprobar que la conexion no se haya caido.

### mqtt.ino
En este se encuentran todas las funciones relacionadas con la comunicacion MQTT. La funcion `mqtt_connect()`, encargada de configurar el cliente MQTT y especificar la función que va a ser el callback y realizar la conexión MQTT, `mqtt_reconnect`, encargada de conectarse al broker elegido, `mqttCallback`, esta funcion se invoca automáticamente al recibir datos por algunos de los topics correspondientes y realiza el cosdigo pertinente dependiendo del mensaje recibido, `mqtt_publish`, esta publica por el topic correspondiente, mientras que `mqtt_subscribe` se subscribe al topic correspondiente. Por ultimo encontramos `mqtt_loop`, encargada de llamar al `mqttClient.loop()` para mantener la conexión activa, así como realizar la reconexión automática mediante  `mqtt_reconnect()`.

### a_logger.ino
En este archivo se encuentran una serie de definiciones que indican tipos de mensajes.

## Integracion con el resto del proyecto

RoboDK recibe señales de emergencia o inicio provenientes de la ESP y este envia mensajes de planta activa o inactiva a la ESP.

## Guia de compilacion
### Requisitos previos
  * PC con Windows, Linux o macOS
  * Cable USB-C para conectar el ESP32-S3
  * Arduino IDE con configuracion de la ESP32-S3

### Librerias externas (necesarias de instalar)
  * PubSubClient - Nick O'Leary
  * ArduinoJson - Benoit Blanchon

### Librerias interna (no necesarias de instalar)
  * WiFi.h

Se requiere cambiar los parametros de NET_SSID y NET_PASSWD por la red que se vaya a usar.

## Autores
SoniaS. Galeas, Marcos Mérida, Lucía Remolar y Marta Ros
Grupo PR2‑1-3  
Grado en Informática Industrial y Robótica — UPV  
Curso 2026

